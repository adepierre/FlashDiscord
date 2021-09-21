#if CLASSIC_BUILD
#include <string>
#else
#include <FlashRuntimeExtensions.h>
#endif

#if CLASSIC_BUILD
void ASInitDiscord(int64_t appId);
const bool IsUserDefined();
const std::string AsGetCurrentUser();
void AsRunDiscordCallbacks();
#else
FREObject ASInitDiscord(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
FREObject ASGetCurrentUser(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
FREObject ASRunDiscordCallbacks(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
#endif