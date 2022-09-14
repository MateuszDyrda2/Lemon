#include <gtest/gtest.h>

#include <world2/component_pool.h>

namespace ComponentPoolTests {
using namespace lemon;
TEST(ComponentPoolTest, Constructior)
{
    component_pool p(componentdef{ 0, sizeof(int) });
}

TEST(ComponentPoolTest, AddComponent)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    int value = 3;
    entityid ent(5);
    auto res = p.cadd_component(ent, &value);
    ASSERT_EQ(value, *static_cast<int*>(res));
    ASSERT_EQ(1, p.get_count());
    ASSERT_EQ(value, *static_cast<int*>(p.get_component(ent)));
}

TEST(ComponentPoolTest, AddEmptyComponent)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    entityid ent(1);
    auto* res = p.add_component(ent);
    ASSERT_EQ(1, p.get_count());
}

TEST(ComponentPoolTest, CXXAddComponent)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    entityid ent(1);
    const int val = 3;
    auto& res     = p.add_component(ent, val);
    ASSERT_EQ(1, p.get_count());
    ASSERT_EQ(val, res);
    ASSERT_EQ(val, *static_cast<int*>(p.get_component(ent)));
}

TEST(ComponentPoolTest, CXXAddComponentMove)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    entityid ent(1);
    int val   = 3;
    auto& res = p.add_component(ent, std::move(val));
    ASSERT_EQ(1, p.get_count());
    ASSERT_EQ(val, res);
    ASSERT_EQ(val, *static_cast<int*>(p.get_component(ent)));
}

TEST(ComponentPoolTest, RemoveComponent)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    entityid ent(1), ent2(2);
    p.add_component(ent, 2);
    p.add_component(ent2, 3);
    ASSERT_EQ(2, p.get_count());
    p.remove_component(ent);
    ASSERT_EQ(1, p.get_count());
    p.remove_component(ent2);
    ASSERT_EQ(0, p.get_count());
}

TEST(ComponentPoolTest, RemoveEntity)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    entityid ent(1), ent2(2);
    p.add_component(ent, 2);
    p.add_component(ent2, 3);
    ASSERT_EQ(2, p.get_count());
    p.remove_entity(ent);
    ASSERT_EQ(1, p.get_count());
    p.remove_entity(ent2);
    ASSERT_EQ(0, p.get_count());
}

TEST(ComponentPoolTest, HasComponent)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    entityid ent(1);
    p.add_component(ent, 2);
    ASSERT_TRUE(p.has_component(ent));
}

TEST(ComponentPoolTest, GetAll)
{
    component_pool p(componentdef{ 0, sizeof(int) });
    for(int i = 0; i < 10; ++i) p.add_component(entityid(i), i);
    ASSERT_EQ(10, p.get_count());
    for(int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(i, p.get_component<int>(entityid(i)));
    }
}
}