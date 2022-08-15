#include <gtest/gtest.h>

#include <core/logger.h>

#include <sstream>

using namespace lemon;
class LoggerTest : public ::testing::Test
{
  protected:
    static void SetUpTestSuite()
    {
        logger::stream(stream);
    }

    void TearDown() override
    {
        stream.str(std::string());
    }

  public:
    inline static std::ostringstream stream;
};
TEST_F(LoggerTest, LogInfo_BasicLog)
{
    std::string message = "Testing123";
    logger::info("{}", message);
    ASSERT_EQ(stream.str(), "[INFO]:\t" + message + "\n");
}
TEST_F(LoggerTest, LogInfo_LogWithArguments)
{
    logger::info("test nr {}", 1);
    ASSERT_EQ(stream.str(), "[INFO]:\ttest nr 1\n");
}
TEST_F(LoggerTest, LogWarn_BasicLog)
{
    std::string message = "Testing123";
    logger::warn("{}", message);
    ASSERT_EQ(stream.str(), "[WARN]:\t" + message + "\n");
}
TEST_F(LoggerTest, LogWarn_LogWithArguments)
{
    logger::warn("test nr {}", 1);
    ASSERT_EQ(stream.str(), "[WARN]:\ttest nr 1\n");
}
TEST_F(LoggerTest, LogError_BasicLog)
{
    std::string message = "Testing123";
    logger::error("{}", message);
    ASSERT_EQ(stream.str(), "[ERROR]:\t" + message + "\n");
}
TEST_F(LoggerTest, LogError_LogWithArguments)
{
    logger::error("test nr {}", 1);
    ASSERT_EQ(stream.str(), "[ERROR]:\ttest nr 1\n");
}
