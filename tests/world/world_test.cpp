#include <gtest/gtest.h>

#include <world2/world.h>

namespace WorldTests {
using namespace lemon;
TEST(WorldTest, Constructor)
{
    world w;
}

TEST(WorldTest, CreateEntity)
{
    world w;
    auto id = w.create();
    ASSERT_EQ(id.alive, 1);
    ASSERT_EQ(id.enabled, 1);
    ASSERT_EQ(id.version, 0);
    ASSERT_EQ(id.id, 0);
}

TEST(WorldTest, DestroyEntity)
{
    world w;
    auto id = w.create();
    w.destroy(id);
    ASSERT_FALSE(w.is_alive(id));
    ASSERT_FALSE(w.is_enabled(id));
}

TEST(WorldTest, AddComponent)
{
    world w;
    auto id  = w.create();
    auto res = w.add(id, world::type<int>::get());
    ASSERT_NE(res, nullptr);
}

TEST(WorldTest, HasAll)
{
    world w;
    auto id = w.create();
    auto t1 = world::type<int>::get();
    auto t2 = world::type<float>::get();
    auto t3 = world::type<double>::get();
    w.add(id, t1);
    w.add(id, t2);
    w.add(id, t3);
    ASSERT_TRUE(w.all(id, { t1.id }));
    ASSERT_TRUE(w.all(id, { t1.id, t2.id }));
    ASSERT_TRUE(w.all(id, { t1.id, t3.id }));
    ASSERT_TRUE(w.all(id, { t1.id, t2.id, t3.id }));
    ASSERT_FALSE(w.all(id, { t1.id, t2.id, t3.id, world::type<char>::id() }));
}

TEST(WorldTest, HasAny)
{
    world w;
    auto id = w.create();
    auto t1 = world::type<int>::get();
    auto t2 = world::type<float>::get();
    auto t3 = world::type<double>::get();
    w.add(id, t1);
    w.add(id, t2);
    w.add(id, t3);
    ASSERT_TRUE(w.any(id, { t1.id }));
    ASSERT_TRUE(w.any(id, { t1.id, t2.id }));
    ASSERT_TRUE(w.any(id, { t1.id, t3.id }));
    ASSERT_TRUE(w.any(id, { t1.id, t2.id, t3.id }));
    ASSERT_TRUE(w.any(id, { t1.id, t2.id, t3.id, world::type<char>::id() }));
    ASSERT_FALSE(w.any(id, { world::type<unsigned char>::id() }));
}

TEST(WorldTest, CXXHasAll)
{
    world w;
    auto id = w.create();
    w.add<int>(id);
    w.add<float>(id);
    w.add<double>(id);
    ASSERT_TRUE(w.all<int>(id));
    ASSERT_TRUE((w.all<int, float>(id)));
    ASSERT_TRUE((w.all<int, double>(id)));
    ASSERT_TRUE((w.all<int, float, double>(id)));
    ASSERT_FALSE((w.all<int, float, double, char>(id)));
}

TEST(WorldTest, CXXHasAny)
{
    world w;
    auto id = w.create();
    w.add<int>(id);
    w.add<float>(id);
    w.add<double>(id);
    ASSERT_TRUE(w.any<int>(id));
    ASSERT_TRUE((w.any<int, float>(id)));
    ASSERT_TRUE((w.any<int, double>(id)));
    ASSERT_TRUE((w.any<int, float, double>(id)));
    ASSERT_TRUE((w.any<int, float, double, char>(id)));
    ASSERT_FALSE(w.any<unsigned char>(id));
}

TEST(WorldTest, AddComponentWithValue)
{
    world w;
    auto id  = w.create();
    int val  = 3;
    auto ret = w.add(id, &val, world::type<int>().get());
    ASSERT_EQ(val, *static_cast<int*>(ret));
    ASSERT_EQ(val, w.get<int>(id));
}
}