#include <river/core/assert.h>
#include <river/core/logger.h>

namespace river {
void logger::print(char const* file, int line, char const* msg, ...)
{
    va_list args;
    va_start(args, msg);
    char buffer[64];
    vsnprintf(buffer, 64, msg, args);
    output("\033[32m", "[LOG]: ", file, line, buffer);
    va_end(args);
}
void logger::warn(char const* file, int line, char const* msg, ...)
{
    va_list args;
    va_start(args, msg);
    char buffer[64];
    vsnprintf(buffer, 64, msg, args);
    output("\033[33m", "[WARN]: ", file, line, buffer);
    va_end(args);
}
void logger::error(char const* file, int line, char const* msg, ...)
{
    va_list args;
    va_start(args, msg);
    char buffer[64];
    vsnprintf(buffer, 64, msg, args);
    output("\033[31m", "[ERROR]: ", file, line, buffer);
    va_end(args);
}
void logger::fatal(char const* file, int line, char const* msg, ...)
{
    va_list args;
    char buffer[64];
    vsnprintf(buffer, 64, msg, args);
    va_start(args, msg);
    output("\033[41m", "[FATAL]: ", file, line, buffer);
    va_end(args);
    RIVER_DB();
}
void logger::output(char const* color, char const* type, char const* file,
                    int line, char const* msg)
{
#if defined(RIVER_LINUX)
    printf("%s%s\t%s\033[0m\n%s:%d\n", color, type, msg, file, line);
#else
    printf("%s\t%s\n%s:%d\n", type, msg, file, line);
#endif // RIVER_LINUX
}
logger& logger::get()
{
    static logger instance;
    return instance;
}
}