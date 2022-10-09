#include <algorithm>
#include <gtest/gtest.h>

#include <physics/bvh_tree.h>

using namespace lemon;
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
    ASSERT_EQ(tree.get_node_count(), 6);
}

TEST(PhysicsBVHTest, RemoveLeaf)
{
    bvh_tree tree;
    tree.insert_leaf(0, { .min = { 0, 0 }, .max = { 1, 1 } });
    tree.insert_leaf(1, { .min = { 1, 1 }, .max = { 2, 2 } });
    ASSERT_EQ(tree.get_node_count(), 2);
    tree.remove_leaf(0);
    tree.remove_leaf(1);
    ASSERT_EQ(tree.get_node_count(), 0);
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
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res.front(), 1);
    auto res2 = tree.query_tree(1);
    ASSERT_EQ(res2.size(), 3);
    ASSERT_TRUE(std::find(res2.begin(), res2.end(), 0));
    ASSERT_TRUE(std::find(res2.begin(), res2.end(), 2));
    ASSERT_TRUE(std::find(res2.begin(), res2.end(), 3));
    auto res3 = tree.query_tree(2);
    ASSERT_EQ(res3.size(), 2);
    ASSERT_TRUE(std::find(res3.begin(), res3.end(), 1));
    ASSERT_TRUE(std::find(res3.begin(), res3.end(), 3));
    auto res4 = tree.query_tree(3);
    ASSERT_EQ(res4.size(), 2);
    ASSERT_TRUE(std::find(res4.begin(), res4.end(), 1));
    ASSERT_TRUE(std::find(res4.begin(), res4.end(), 2));
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
