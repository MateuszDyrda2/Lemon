#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

#define LOG_MESSAGE(...)                                      \
    {                                                         \
        logger::get().print(__FILE__, __LINE__, __VA_ARGS__); \
    }
#define LOG_WARN(...)                                        \
    {                                                        \
        logger::get().warn(__FILE__, __LINE__, __VA_ARGS__); \
    }
#define LOG_ERROR(...)                                        \
    {                                                         \
        logger::get().error(__FILE__, __LINE__, __VA_ARGS__); \
    }
#define LOG_FATAL(...)                                        \
    {                                                         \
        logger::get().fatal(__FILE__, __LINE__, __VA_ARGS__); \
    }

namespace lemon {
class logger
{
  public:
    void print(char const* file, int line, char const* msg, ...);
    void warn(char const* file, int line, char const* msg, ...);
    void error(char const* file, int line, char const* msg, ...);
    void fatal(char const* file, int line, char const* msg, ...);
    static logger& get();

  private:
    void output(char const* color, char const* type, char const* file, int line, char const* msg);
};
}