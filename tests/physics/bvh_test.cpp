#include "physics/detail/physics_utils.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <gtest/gtest.h>

#include <physics/bvh_tree.h>

using namespace lemon;
struct AABB_area_params
{
    detail::AABB arg;
    f32 result;
};
class AABB_area_test : public testing::TestWithParam<AABB_area_params>
{ };
struct AABB_union_params
{
    detail::AABB a, b, res;
};
class AABB_union_test : public testing::TestWithParam<AABB_union_params>
{ };
struct intersects_params
{
    detail::AABB a, b;
    bool res;
};
class intersects_test : public testing::TestWithParam<intersects_params>
{ };

TEST_P(AABB_area_test, AABB_area)
{
    auto&& [arg, res] = GetParam();
    auto area         = detail::AABB_area(arg);
    ASSERT_EQ(res, area);
}
TEST_P(AABB_union_test, AABB_union)
{
    auto&& [a, b, res] = GetParam();
    auto un            = detail::AABB_union(a, b);
    ASSERT_EQ(un.min.x, res.min.x);
    ASSERT_EQ(un.min.y, res.min.y);
    ASSERT_EQ(un.max.x, res.max.x);
    ASSERT_EQ(un.max.y, res.max.y);
}
TEST_P(intersects_test, intersects)
{
    auto&& [a, b, res] = GetParam();
    auto r             = detail::intersects(a, b);
    ASSERT_EQ(r, res);
}
const AABB_area_params params1[] = {
    { .arg = { .min = { 0, 0 }, .max = { 0, 0 } }, .result = 0 },
    { .arg = { .min = { 0, 0 }, .max = { 5, 5 } }, .result = 5 * 5 },
    { .arg = { .min = { -5, -5 }, .max = { -2, 12 } }, .result = 3 * 17 },
    { .arg = { .min = { -5, -1 }, .max = { 0, 0 } }, .result = 5 * 1 },
    { .arg = { .min = { -12.21, -21.22 }, .max = { 12, 22 } }, .result = 24.21 * 43.22 },
};
INSTANTIATE_TEST_SUITE_P(AABB_areaTest, AABB_area_test,
                         testing::ValuesIn(params1));
const AABB_union_params params2[] = {
    { .a = { { 0, 0 }, { 5, 5 } }, .b = { { 6, 6 }, { 10, 10 } }, .res = { { 0, 0 }, { 10, 10 } } },
    { .a = { { -12, -12 }, { 0, 0 } }, .b = { { -5, -5 }, { 1, 1 } }, .res = { { -12, -12 }, { 1, 1 } } },
    { .a = { { 4, 4 }, { 5, 5 } }, .b = { { -1, -1 }, { 0, 0 } }, .res = { { -1, -1 }, { 5, 5 } } },
    { .a = { { -5, 2 }, { 5, 22 } }, .b = { { -3, 4 }, { 1, 5 } }, .res = { { -5, 2 }, { 5, 22 } } },
};
INSTANTIATE_TEST_SUITE_P(AABB_unionTest, AABB_union_test,
                         testing::ValuesIn(params2));
const intersects_params params3[] = {
    { .a = { { 0, 0 }, { 2, 2 } }, .b = { { 3, 3 }, { 4, 4 } }, .res = false },
    { .a = { { 5, 5 }, { 7, 7 } }, .b = { { 6, 6 }, { 8, 8 } }, .res = true },
    { .a = { { 1.2, 1.3 }, { 1.3, 5 } }, .b = { { 1.25, 7 }, { 5, 8 } }, .res = false },
    { .a = { { 1, 1 }, { 4, 4 } }, .b = { { 4, 4 }, { 5, 5 } }, .res = true },
};
INSTANTIATE_TEST_SUITE_P(intersectsTest, intersects_test,
                         testing::ValuesIn(params3));

TEST(PhysicsBVHTest, Constructor)
{
    bvh_tree tree;
}
TEST(PhysicsBVHTest, InsertLeaf)
{
    bvh_tree tree;
    tree.insert_leaf(0, { .min = { 0, 0 }, .max = { 1, 1 } });
    tree.insert_leaf(1, { .min = { 1, 1 }, .max = { 2, 2 } });
    tree.insert_leaf(2, { .min = { 4, 3 }, .max = { 5, 4 } });
    tree.insert_leaf(3, { .min = { 2, 1 }, .max = { 3, 4 } });
    tree.insert_leaf(4, { .min = { 1, 2 }, .max = { 4, 3 } });
    tree.insert_leaf(5, { .min = { 4, 8 }, .max = { 5, 9 } });
}

TEST(PhysicsBVHTest, RemoveLeaf)
{
    bvh_tree tree;
    tree.insert_leaf(0, { .min = { 0, 0 }, .max = { 1, 1 } });
    tree.insert_leaf(1, { .min = { 1, 1 }, .max = { 2, 2 } });
    tree.remove_leaf(0);
    tree.remove_leaf(1);
}

TEST(PhysicsBVHTest, UpdateLeaf)
{
    bvh_tree tree;
    tree.insert_leaf(0, { .min = { 0, 0 }, .max = { 1, 1 } });
    tree.update_leaf(0, { .min = { 1, 1 }, .max = { 2, 2 } });
}

TEST(PhysicsBVHTest, QueryTreeOverlapping)
{
    bvh_tree tree;
    tree.insert_leaf(0, { .min = { 0, 0 }, .max = { 1, 1 } });
    tree.insert_leaf(1, { .min = { 0.5, 0 }, .max = { 1.5, 1 } });
    auto res = tree.query_tree(0);
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res.front(), 1);
    auto res2 = tree.query_tree(1);
    ASSERT_EQ(res2.size(), 1);
    ASSERT_EQ(res2.front(), 0);
}

TEST(PhysicsBVHTest, QueryTreeOverlappingMultiple)
{
    bvh_tree tree;
    tree.insert_leaf(0, { .min = { 0, 0 }, .max = { 4, 2 } });
    tree.insert_leaf(1, { .min = { 3, 1 }, .max = { 6, 3 } });
    tree.insert_leaf(2, { .min = { 5, 2 }, .max = { 8, 4 } });
    tree.insert_leaf(3, { .min = { 4, 1 }, .max = { 10, 5 } });
    auto res = tree.query_tree(0);
    ASSERT_EQ(res.size(), 2);
    ASSERT_TRUE(std::find(res.begin(), res.end(), 1) != res.end());
    ASSERT_TRUE(std::find(res.begin(), res.end(), 3) != res.end());
    auto res2 = tree.query_tree(1);
    ASSERT_EQ(res2.size(), 3);
    ASSERT_TRUE(std::find(res2.begin(), res2.end(), 0) != res2.end());
    ASSERT_TRUE(std::find(res2.begin(), res2.end(), 2) != res2.end());
    ASSERT_TRUE(std::find(res2.begin(), res2.end(), 3) != res2.end());
    auto res3 = tree.query_tree(2);
    ASSERT_EQ(res3.size(), 2);
    ASSERT_TRUE(std::find(res3.begin(), res3.end(), 1) != res3.end());
    ASSERT_TRUE(std::find(res3.begin(), res3.end(), 3) != res3.end());
    auto res4 = tree.query_tree(3);
    ASSERT_EQ(res4.size(), 3);
    ASSERT_TRUE(std::find(res4.begin(), res4.end(), 0) != res4.end());
    ASSERT_TRUE(std::find(res4.begin(), res4.end(), 1) != res4.end());
    ASSERT_TRUE(std::find(res4.begin(), res4.end(), 2) != res4.end());
}

TEST(PhysicsBVHTest, QueryTreeNotOverlapping)
{
    bvh_tree tree;
    tree.insert_leaf(0, { .min = { 0, 0 }, .max = { 1, 1 } });
    tree.insert_leaf(1, { .min = { 1.5, 0 }, .max = { 2.5, 1 } });
    auto res = tree.query_tree(0);
    ASSERT_EQ(res.size(), 0);
    auto res2 = tree.query_tree(1);
    ASSERT_EQ(res.size(), 0);
}
