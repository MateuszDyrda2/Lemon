#pragma once

#include "assert.h"
#include "defines.h"

#include <cstdarg>
#include <cstdio>
#include <string>

#define LOG_MESSAGE(...)                                \
    {                                                   \
        logger::print(__FILE__, __LINE__, __VA_ARGS__); \
    }
#define LOG_WARN(...)                                  \
    {                                                  \
        logger::warn(__FILE__, __LINE__, __VA_ARGS__); \
    }
#define LOG_ERROR(...)                                  \
    {                                                   \
        logger::error(__FILE__, __LINE__, __VA_ARGS__); \
    }
#define LOG_FATAL(...)                                  \
    {                                                   \
        logger::fatal(__FILE__, __LINE__, __VA_ARGS__); \
    }

namespace lemon {
class LEMON_PUBLIC logger
{
  public:
    static void print(char const* file, int line, char const* msg, ...);
    static void warn(char const* file, int line, char const* msg, ...);
    static void error(char const* file, int line, char const* msg, ...);
    static void fatal(char const* file, int line, char const* msg, ...);

    static void pprint(std::string message);
    static void pwarn(std::string message);
    static void perror(std::string message);
    static void pfatal(std::string message);

  private:
    static void poutput(std::string message, const char* color, const char* type);
    static void output(char const* color, char const* type,
                       char const* file, int line, char const* msg);
};
}