#include "native_discord_wrapper.hpp"
#include <csignal>
#include <thread>
#include <chrono>

#include <iostream>

namespace {
    volatile bool interrupted{ false };
}

int main(int argc, char* argv[])
{
    int64_t appIdInt = INSERT_YOUR_DISCORD_APP_ID_HERE;
    ASInitDiscord(appIdInt);

    std::signal(SIGINT, [](int) { interrupted = true; });

    while (!interrupted)
    {
        AsRunDiscordCallbacks();
        if (IsUserDefined())
        {
            std::cout << "Username: " << AsGetCurrentUser() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}