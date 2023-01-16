#include <gtest/gtest.h>

#include <core/hash_string.h>

using namespace lemon;
TEST(HashStringTest, TestCreation)
{
    auto val = hashstr("name");
    ASSERT_EQ(val, hashstr("name"));
    constexpr auto v = hashstr("consteval");
    ASSERT_NE(val, v);
}