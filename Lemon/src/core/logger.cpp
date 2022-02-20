#include <lemon/core/logger.h>

namespace lemon {
void logger::print(char const* file, int line, char const* msg, ...)
{
    va_list args;
    va_start(args, msg);
    char buffer[128];
    vsnprintf(buffer, 128, msg, args);
    output("\033[32m", "[LOG]: ", file, line, buffer);
    va_end(args);
}
void logger::warn(char const* file, int line, char const* msg, ...)
{
    va_list args;
    va_start(args, msg);
    char buffer[128];
    vsnprintf(buffer, 128, msg, args);
    output("\033[33m", "[WARN]: ", file, line, buffer);
    va_end(args);
}
void logger::error(char const* file, int line, char const* msg, ...)
{
    va_list args;
    va_start(args, msg);
    char buffer[128];
    vsnprintf(buffer, 128, msg, args);
    output("\033[31m", "[ERROR]: ", file, line, buffer);
    va_end(args);
}
void logger::fatal(char const* file, int line, char const* msg, ...)
{
    va_list args;
    char buffer[128];
    vsnprintf(buffer, 128, msg, args);
    va_start(args, msg);
    output("\033[41m", "[FATAL]: ", file, line, buffer);
    va_end(args);
    LEMON_DB();
}
void logger::output(char const* color, char const* type, char const* file,
                    int line, char const* msg)
{
#if defined(LEMON_LINUX)
    printf("%s%s\t%s\033[0m\n%s:%d\n", color, type, msg, file, line);
#else
    printf("%s\t%s\n%s:%d\n", type, msg, file, line);
#endif // LEMON_LINUX
}
}