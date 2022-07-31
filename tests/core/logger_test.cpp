#include <gtest/gtest.h>

#include <lemon/core/logger.h>

#include <sstream>

using namespace lemon;
TEST(LogInfo, BasicLog)
{
    std::ostringstream stream;
    std::string message = "Testing123";
    logger::stream(stream);
    logger::info("{}", message);
    ASSERT_EQ(stream.str(), "[INFO]:\t" + message + "\n");
}
TEST(LogInfo, LogWithArguments)
{
    std::ostringstream stream;
    logger::stream(stream);
    logger::info("test nr {}", 1);
    ASSERT_EQ(stream.str(), "[INFO]:\ttest nr 1\n");
}
TEST(LogWarn, BasicLog)
{
    std::ostringstream stream;
    std::string message = "Testing123";
    logger::stream(stream);
    logger::warn("{}", message);
    ASSERT_EQ(stream.str(), "[WARN]:\t" + message + "\n");
}
TEST(LogWarn, LogWithArguments)
{
    std::ostringstream stream;
    logger::stream(stream);
    logger::warn("test nr {}", 1);
    ASSERT_EQ(stream.str(), "[WARN]:\ttest nr 1\n");
}
TEST(LogError, BasicLog)
{
    std::ostringstream stream;
    std::string message = "Testing123";
    logger::stream(stream);
    logger::error("{}", message);
    ASSERT_EQ(stream.str(), "[ERROR]:\t" + message + "\n");
}
TEST(LogError, LogWithArguments)
{
    std::ostringstream stream;
    logger::stream(stream);
    logger::error("test nr {}", 1);
    ASSERT_EQ(stream.str(), "[ERROR]:\ttest nr 1\n");
}
