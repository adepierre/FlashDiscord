cmake_minimum_required(VERSION 3.10)

# set the project name
project(NativeDiscordWrapper)

set(AIR_PATH "AIR_PATH_NOT_FOUND" CACHE PATH "Path to air sdk")
option(ANE_BUILD "If ON, build with ANE support" OFF)

if(NOT ANE_BUILD)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

file(GLOB DISCORD_FILES discord_game_sdk/cpp/*)

# add the library
add_library(NativeDiscordWrapper SHARED src/native_discord_wrapper.cpp include/native_discord_wrapper.hpp ${DISCORD_FILES})

if(ANE_BUILD)
    target_include_directories(NativeDiscordWrapper PUBLIC ${AIR_PATH}/include)
endif()

target_include_directories(NativeDiscordWrapper PUBLIC include)
target_include_directories(NativeDiscordWrapper PRIVATE discord_game_sdk/cpp)

if(NOT ANE_BUILD)
    target_compile_definitions(NativeDiscordWrapper PUBLIC CLASSIC_BUILD=1)
endif()

if(ANE_BUILD)
    target_link_libraries(NativeDiscordWrapper PRIVATE ${AIR_PATH}/lib/win/FlashRuntimeExtensions.lib)
endif()
target_link_libraries(NativeDiscordWrapper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/discord_game_sdk/lib/x86/discord_game_sdk.dll.lib)

set_target_properties( NativeDiscordWrapper
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

add_custom_command(TARGET NativeDiscordWrapper POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
    ${CMAKE_CURRENT_SOURCE_DIR}/discord_game_sdk/lib/x86/discord_game_sdk.dll 
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>/discord_game_sdk.dll
)

# add the executable
if(NOT ANE_BUILD)
    add_executable(NativeDiscordWrapperTest src/main.cpp)
    target_link_libraries(NativeDiscordWrapperTest PUBLIC NativeDiscordWrapper)
    
    set_target_properties( NativeDiscordWrapperTest
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)
endif()