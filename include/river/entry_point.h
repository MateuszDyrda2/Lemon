#pragma once

#include "engine.h"

using namespace river;
#define GAME_RUN(GAME_NAME)                   \
    int main(int argc, char** argv)           \
    {                                         \
        auto eng = new GAME_NAME(argc, argv); \
        while(eng->update())                  \
            ;                                 \
        delete eng;                           \
        return 0;                             \
    }
