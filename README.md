# FlashDiscord

Simple proof of concept showing how to build and use an AIR Native Extension, or ANE. The concept is applied to the Discord SDK C library, which means that we could theoretically use it to add a discord voice chat in any Flash app running on AIR.

This example is more like guidelines and should not be used "as is": it was developped with a specific OS/C++/AIR environment, and it's unlikely you'll end up with the exact same one. If you've already played with AIR and C++ you should be able to understand what's goind on though.

## Prerequisites

- Working AIR dev environment (``AS3, MXML, and SWF Extension Pack`` on VS Code)
- C++ compiler (MSVC++ 14.2)
- [Cmake](https://cmake.org/)
- Discord SDK

It is forbidden to redistribute the sources of the discord SDK, so you first need to download and copy the latest version of the SDK from https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip and extract it in NativeDiscordWrapper.

NativeDiscordWrapper should have the following contents:
- discord_game_sdk/
- include/
- src/
- CMakeLists.txt

## NativeDiscordWrapper

Two C++ projects that can be built using CMake:

- a C++ dll that is our wrapper around Discord stuff. If ANE_BUILD is set in CMake, it is built with the AIR stuff to be called from AIR apps. Otherwise it is a standard C++ library.
- a C++ exe, just a main with a few lines. Used to test that the wrapped functions work the way we want.

## AIRDiscordLibrary

ActionScript wrapper around the C++ dll. It has a copy of all functions we want to call in the dll, and can react to events sent by the dll. Compiled to a .xml and .swc file.

## ANE Packing

The ``pack.bat`` file will gather all the files and call the commands needed to create the ANE file. This step can also be performed by hand. If you're not on Windows, you might have to use different commands, but it should be quite easy as it only uses basic stuff. Once the .ane file is generated, it has to be copied to ``AIRDiscordExe/extensions/``.

Warning: the debug version of the dll is copied, if you want to use the Release one or if you're not on Windows, you'll need to change the path.

## AIRDiscordExe

Main AIR app using the AIRDiscordLibrary developped earlier. Before building, you might want to change the .pfx file with the password AIR thing in ``asconfig.json``.

The project is built as a standalone AIR app, with a very inspired name: Main.exe. In order to work, you must copy ``NativeDiscordWrapper/discord_game_sdk/lib/x86/discord_game_sdk.dll`` next to ``Main.exe``. If discord is launched on the same computer, it should display the current user name.

AS3 doesn't have 64 bits int, while discord uses them everywhere, so they are passed as strings instead.

## License

MIT
