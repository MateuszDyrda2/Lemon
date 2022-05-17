#pragma once

#if defined(LEMON_DEBUG)
#    if defined(LEMON_WINDOWS)
#        define LEMON_DB() __debugbreak()
#    elif defined(LEMON_LINUX)
#        include <signal.h>
#        define LEMON_DB() raise(SIGTRAP)
#    else
#        error "No debug break for the platform"
#    endif // LEMON_WINDOWS
#    define lemon_assert(arg)      \
        {                          \
            if(!(arg)) LEMON_DB(); \
        }
#else
#    define LEMON_DB()
#    define lemon_assert(arg)
#endif // LEMON_DEBUG
#define lemon_sassert(arg, msg) static_assert(arg, msg)