#pragma once

#ifdef LEMON_WINDOWS
#    ifdef LEMON_EXPORTS
#        define LEMON_API __declspec(dllexport)
#    else
#        define LEMON_API __declspec(dllimport)
#    endif // !LEMONCORE_EXPORTS

#else
#    define LEMON_API
#endif
