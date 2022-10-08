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

#define EXP(X)                                             X
#define CONCAT_IMPL(_1, _2)                                _1##_2
#define CONCAT(_1, _2)                                     CONCAT_IMPL(_1, _2)
#define GET_ARG_COUNT(...)                                 _NARGS(PREFIX(__VA_ARGS__))
#define _NARGS(...)                                        EXP(LASTOF(__VA_ARGS__, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define LASTOF(a, b, c, d, e, f, g, h, i, j, k, l, m, ...) m
#define PREFIX(...)                                        0, ##__VA_ARGS__
