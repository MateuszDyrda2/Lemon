#pragma once

#if defined(RIVER_DEBUG)
#    if defined(RIVER_WINDOWS)
#        define RIVER_DB() __debugbreak()
#    elif defined(RIVER_LINUX)
#        include <signal.h>
#        define RIVER_DB() raise(SIGTRAP)
#    else
#        error "No debug break for the platform"
#    endif // RIVER_WINDOWS
#    define RIVER_ASSERT(arg)      \
        {                          \
            if(!(arg)) RIVER_DB(); \
        }
#else
#    define RIVER_DB()
#    define RIVER_ASSERT(arg)
#endif // RIVER_DEBUG
#define RIVER_SASSERT(arg, msg) static_assert(arg, msg)