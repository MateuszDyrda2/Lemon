/** @file logger.h
 * @brief Utility class for logging
 */
#pragma once

#include "assert.h"

#include <iostream>
#include <ostream>

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ostream.h>

namespace lemon {
/** Class supporting logging to stdout or file */
class logger
{
  public:
    /** @brief Log as info
     * @param message the message to be logger
     * @param args format parameters
     */
    template<class... Args>
    inline static void info(fmt::format_string<Args...> message, Args&&... args)
    {
        fmt::print(
            stream(),
            "[INFO]:\t");
        fmt::print(
            stream(),
            message, std::forward<Args>(args)...);
        fmt::print(
            stream(),
            "\n");
    }
    /** @brief Log as warning
     * @param message the message to be logger
     * @param args format parameters
     */
    template<class... Args>
    inline static void warn(fmt::format_string<Args...> message, Args&&... args)
    {
        fmt::print(
            stream(),
            "[WARN]:\t");
        fmt::print(
            stream(),
            message, std::forward<Args>(args)...);
        fmt::print(
            stream(),
            "\n");
    }
    /** @brief Log as error
     * @param message the message to be logger
     * @param args format parameters
     */
    template<class... Args>
    inline static void error(fmt::format_string<Args...> message, Args&&... args)
    {
        fmt::print(
            stream(),
            "[ERROR]:\t");
        fmt::print(
            stream(),
            message, std::forward<Args>(args)...);
        fmt::print(
            stream(),
            "\n");
    }
    /** @brief Log as fatal and stop program execution
     * @param message the message to be logger
     * @param args format parameters
     */
    template<class... Args>
    inline static void fatal(fmt::format_string<Args...> message, Args&&... args)
    {
        fmt::print(
            stream(),
            "[FATAL]:\t");
        fmt::print(
            stream(),
            message, std::forward<Args>(args)...);
        fmt::print(
            stream(),
            "\n");
        lemon_assert(false);
    }
    /** @brief Set or get the output stream (default cout)
     * @param os output stream
     * @return the current output stream
     */
    inline static std::ostream& stream(std::ostream& os = std::cout)
    {
        static std::ostream& s_os = os;
        return s_os;
    }
};
} // namespace lemon
