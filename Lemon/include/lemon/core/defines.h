#pragma once

#if defined(LEMON_WINDOWS)
#    define LEMON_LIB_EXPORT __declspec(dllexport)
#    define LEMON_LIB_IMPORT __declspec(dllimport)
#elif defined(LEMON_LINUX)
#    define LEMON_LIB_EXPORT __attribute__((visibility("default")))
#    define LEMON_LIB_IMPORT
#else
#    define LEMON_LIB_EXPORT
#    define LEMON_LIB_IMPORT
#    pragma warning Unknown platform
#endif

#if defined(EXPORT_LEMON)
#    define LEMON_PUBLIC LEMON_LIB_EXPORT
#else
#    define LEMON_PUBLIC LEMON_LIB_IMPORT
#endif
