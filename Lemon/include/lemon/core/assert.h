#pragma once

#ifndef LEMON_RESOURCE_PATH
#    define LEMON_RESOURCE_PATH "resources/"
#endif
#if defined(LEMON_DEBUG)
#    if defined(LEMON_WINDOWS)
#        define LEMON_DB() __debugbreak()
#    elif defined(LEMON_LINUX)
#        include <signal.h>
#        define LEMON_DB() raise(SIGTRAP)
#    else
#        error "No debug break for the platform"
#    endif // LEMON_WINDOWS
#    define LEMON_ASSERT(arg)      \
        {                          \
            if(!(arg)) LEMON_DB(); \
        }
#else
#    define LEMON_DB()
#    define LEMON_ASSERT(arg)
#endif // LEMON_DEBUG
#define LEMON_SASSERT(arg, msg) static_assert(arg, msg)