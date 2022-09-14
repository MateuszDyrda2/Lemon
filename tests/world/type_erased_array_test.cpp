#include <gtest/gtest.h>

#include <world2/type_erased_array.h>

namespace TypeErasedArrayTests {
using namespace lemon;
TEST(TypeErasedArrayTest, Constructor)
{
    type_erased_array a(sizeof(int));
}

TEST(TypeErasedArrayTest, PushWithArgument)
{
    type_erased_array a(sizeof(int));
    int test = 3;
    auto res = a.cpush(&test);
    ASSERT_EQ(test, *static_cast<int*>(res));
    ASSERT_EQ(1, a.get_size());
}

TEST(TypeErasedArrayTest, PushNoArgument)
{
    type_erased_array a(sizeof(int));
    auto res = a.push();
    ASSERT_NE(res, nullptr);
    ASSERT_EQ(1, a.get_size());
}

TEST(TypeErasedArrayTest, CXXPushWithConstruct)
{
    struct arg
    {
        int val;
    };
    type_erased_array a(sizeof(int));
    auto& res = a.push<arg>(3);
    ASSERT_EQ(res.val, 3);
    ASSERT_EQ(1, a.get_size());
}

TEST(TypeErasedArrayTest, CXXPushWithCopy)
{
    type_erased_array a(sizeof(int));
    const int val = 3;
    auto& res     = a.push(val);
    ASSERT_EQ(res, val);
    ASSERT_EQ(1, a.get_size());
}

TEST(TypeErasedArrayTest, CXXPushWithMove)
{
    type_erased_array a(sizeof(int));
    auto& res = a.push(3);
    ASSERT_EQ(res, 3);
    ASSERT_EQ(1, a.get_size());
}

TEST(TypeErasedArrayTest, SwapAndDiscard)
{
    type_erased_array a(sizeof(int));
    a.push(1);
    a.push(2);
    a.push(3);
    a.swap_and_discard(0);
    ASSERT_EQ(2, a.get_size());
    ASSERT_EQ(*static_cast<int*>(a.at(0)), 3);
    ASSERT_EQ(*static_cast<int*>(a.at(1)), 2);
}

TEST(TypeErasedArrayTest, Get)
{
    type_erased_array a(sizeof(int));
    for(int i = 0; i < 10; ++i) a.cpush(&i);
    int* back = static_cast<int*>(a.get());
    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(*static_cast<int*>(a.at(i)), i);
    }
    ASSERT_EQ(10, a.get_size());
}
}
