#pragma once

#include "engine.h"

using namespace river;
#define RIVER_GAME_DECL(GAME_NAME)            \
    class GAME_NAME : public engine           \
    {                                         \
      public:                                 \
        GAME_NAME(int argc, char** argv);     \
        ~GAME_NAME();                         \
    };                                        \
    int main(int argc, char** argv)           \
    {                                         \
        auto eng = new GAME_NAME(argc, argv); \
        eng->run();                           \
        return 0;                             \
    }

#define RIVER_ON_CREATE(GAME_NAME) \
    GAME_NAME::GAME_NAME(int argc, char** argv): engine(argc, argv)
#define RIVER_ON_DESTROY(GAME_NAME) \
    GAME_NAME::~GAME_NAME()
