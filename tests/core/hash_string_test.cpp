#include <gtest/gtest.h>

#include <core/hash_string.h>

using namespace lemon;
TEST(HashStringTest, TestCreation)
{
    auto val = hash_string("name");
    ASSERT_EQ(val, hash_string("name"));
    ASSERT_NE(val, 0);
    constexpr auto v = hash_string("consteval");
    ASSERT_NE(val, v);
}