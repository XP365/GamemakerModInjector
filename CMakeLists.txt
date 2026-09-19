cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(GameMakerModInjector)

FetchContent_Declare(
        MinHook
        GIT_REPOSITORY https://github.com/tsudakageyu/minhook
        GIT_TAG        v1.3.4
)
FetchContent_MakeAvailable(MinHook)

add_library(GameMakerModInjector SHARED
        library.cpp
        proxy_exports.cpp
        Logging.cpp
        Logging.h
)

target_compile_options(GameMakerModInjector PRIVATE
        -fno-exceptions
        -fno-rtti
        -fno-unwind-tables
        -fno-asynchronous-unwind-tables
)

target_link_options(GameMakerModInjector PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
)

target_link_libraries(GameMakerModInjector PRIVATE
        minhook
        dbghelp
)

#force output name
set_target_properties(GameMakerModInjector PROPERTIES
        PREFIX ""
        OUTPUT_NAME "version"
        SUFFIX ".dll"
)

set_target_properties(GameMakerModInjector PROPERTIES
        #Set the path to your DELTARUNE Install
        RUNTIME_OUTPUT_DIRECTORY "/mnt/daf5e267-e777-4d37-99dd-cbb354b77f13/SteamLibrary/steamapps/common/DELTARUNE/"
)
