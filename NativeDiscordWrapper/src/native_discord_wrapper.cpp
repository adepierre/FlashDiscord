#include <assert.h>
#include <sstream>


#include "discord.h"

#include "native_discord_wrapper.hpp"

struct DiscordState {
	discord::User currentUser;

	std::unique_ptr<discord::Core> core;
#if CLASSIC_BUILD
	bool userDefined = false;
#else
	FREContext context;
#endif
};

DiscordState state = {};


#if CLASSIC_BUILD


void ASInitDiscord(int64_t appId)
{
	discord::Core* core{};
	auto result = discord::Core::Create(appId, DiscordCreateFlags_Default, &core);
	state.core.reset(core);

	if (state.core)
	{
		state.core->UserManager().OnCurrentUserUpdate.Connect([]()
			{
				state.core->UserManager().GetCurrentUser(&state.currentUser);
				state.userDefined = true;
			});
	}
}

const bool IsUserDefined()
{
	return state.userDefined;
}

const std::string AsGetCurrentUser()
{
	return state.currentUser.GetUsername();
}

void AsRunDiscordCallbacks()
{
	if (state.core)
	{
		state.core->RunCallbacks();
	}
}

#else
FREObject ASInitDiscord(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	enum
	{
		ARG_INT_STRING = 0,
		ARG_COUNT
	};
	assert(ARG_COUNT == argc);

	uint32_t strLength = 0;
	const uint8_t* appIdString = NULL;
	FREResult status = FREGetObjectAsUTF8(argv[0], &strLength,  &appIdString);

	if (status != FRE_OK)
	{
		// Uh oh
	}

	discord::Core* core{};
	auto result = discord::Core::Create(stoll(std::string((const char*)appIdString)), DiscordCreateFlags_Default, &core);
	state.core.reset(core);
	state.context = ctx;

	if (state.core)
	{
		state.core->UserManager().OnCurrentUserUpdate.Connect([]()
			{
				state.core->UserManager().GetCurrentUser(&state.currentUser);
				FREDispatchStatusEventAsync(state.context, (const uint8_t*)("USER_UPDATED"), (const uint8_t*)(""));
			});
	}

	FREObject retObj;

	FRENewObjectFromInt32(static_cast<int>(result), &retObj);

	return retObj;
}

FREObject ASGetCurrentUser(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	enum
	{
		ARG_COUNT = 0
	};
	assert(ARG_COUNT == argc);

	FREObject retObj;
	FRENewObjectFromUTF8(strlen(state.currentUser.GetUsername()), (const uint8_t*)(state.currentUser.GetUsername()), &retObj);

	return retObj;
}

FREObject ASRunDiscordCallbacks(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{
	enum
	{
		ARG_COUNT = 0
	};
	assert(ARG_COUNT == argc);

	FREObject retObj;
	if (state.core)
	{
		state.core->RunCallbacks();
		FRENewObjectFromBool(1, &retObj);
	}
	else
	{
		FRENewObjectFromBool(0, &retObj);
	}

	return retObj;
}

void contextInitializer(
	void* extData,
	const uint8_t* ctxType,
	FREContext				   ctx,
	uint32_t* numFunctionsToSet,
	const FRENamedFunction** functionsToSet)
{
	// Create mapping between function names and pointers in an array of FRENamedFunction.
	// These are the functions that you will call from ActionScript -
	// effectively the interface of your native library.
	// Each member of the array contains the following information:
	// { function name as it will be called from ActionScript,
	//   any data that should be passed to the function,
	//   a pointer to the implementation of the function in the native library }
	static FRENamedFunction extensionFunctions[] =
	{
		{ (const uint8_t*)"as_InitDiscord",        NULL, &ASInitDiscord },
		{ (const uint8_t*)"as_GetCurrentUser",        NULL, &ASGetCurrentUser },
		{ (const uint8_t*)"as_RunDiscordCallbacks",        NULL, &ASRunDiscordCallbacks },
	};

	// Tell AIR how many functions there are in the array:
	*numFunctionsToSet = sizeof(extensionFunctions) / sizeof(FRENamedFunction);

	// Set the output parameter to point to the array we filled in:
	*functionsToSet = extensionFunctions;
}

void contextFinalizer(FREContext ctx)
{
	state = {};
	return;
}

extern "C"
{
	__declspec(dllexport) void ExtensionInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
	{
		*ctxInitializer = &contextInitializer; // The name of function that will intialize the extension context
		*ctxFinalizer = &contextFinalizer; // The name of function that will finalize the extension context
	}

	__declspec(dllexport) void ExtensionFinalizer(void* extData)
	{
		return;
	}
}
#endif