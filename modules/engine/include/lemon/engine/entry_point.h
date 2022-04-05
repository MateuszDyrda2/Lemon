#pragma once

#include "engine.h"
#include <lemon/core/instrumentor.h>

#define GAME_RUN(GAME_NAME)                                           \
    int main(int argc, char** argv)                                   \
    {                                                                 \
        LEMON_PROFILE_BEGIN("Initialization", "initialization.json"); \
        auto eng = new GAME_NAME(argc, argv);                         \
        LEMON_PROFILE_END();                                          \
        LEMON_PROFILE_BEGIN("Update", "update.json");                 \
        while(eng->update())                                          \
            ;                                                         \
        LEMON_PROFILE_END();                                          \
        LEMON_PROFILE_BEGIN("Cleanup", "cleanup.json");               \
        delete eng;                                                   \
        LEMON_PROFILE_END();                                          \
        return 0;                                                     \
    }
