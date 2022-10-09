#include <gtest/gtest.h>

#include <core/math/math.h>
#include <physics/sat_algorithm.h>

using namespace lemon;
TEST(PhysicsSATTest, OverlappingBoxes)
{
    sat_algorithm sat;
    box_shape a{
        .vertices = {
            { 0.f, 0.f },
            { 1.f, 0.f },
            { 1.f, 1.f },
            { 0.f, 1.f },
        }
    };
    box_shape b{
        .vertices = {
            { 0.5f, 0.f },
            { 1.5f, 0.f },
            { 1.5f, 1.f },
            { 0.5f, 1.f },
        }
    };
    auto res = sat(a, b);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res->axis, (vec2{ 1.f, 0.f }));
    ASSERT_EQ(res->overlap, 0.5f);
}

TEST(PhysicsSATTest, OverlappingNotAxisAlignedBoxes)
{
    sat_algorithm sat;
    box_shape a{
        .vertices = {
            { 2.f, 0.f },
            { 4.f, 1.f },
            { 2.f, 2.f },
            { 0.f, 1.f },
        }
    };
    box_shape b{
        .vertices = {
            { 4.0f, 0.0f },
            { 6.0f, 1.0f },
            { 4.0f, 3.0f },
            { 2.0f, 2.0f },
        }
    };
    auto res = sat(a, b);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res->axis, normalize(vec2{ 2.f, 1.f }));
}

TEST(PhysicsSATTest, NotOverlappingBoxes)
{
    sat_algorithm sat;
    box_shape a{
        .vertices = {
            { 0.f, 0.f },
            { 1.f, 0.f },
            { 1.f, 1.f },
            { 0.f, 1.f },
        }
    };
    box_shape b{
        .vertices = {
            { 1.5f, 0.f },
            { 2.5f, 0.f },
            { 2.5f, 1.f },
            { 1.5f, 1.f },
        }
    };
    auto res = sat(a, b);
    ASSERT_FALSE(res.has_value());
}

TEST(PhysicsSATTest, NotOverlappingNotAxisAlignedBoxes)
{
    sat_algorithm sat;
    box_shape a{
        .vertices = {
            { 2.f, 0.f },
            { 4.f, 1.f },
            { 2.f, 2.f },
            { 0.f, 1.f },
        }
    };
    box_shape b{
        .vertices = {
            { 6.0f, 0.0f },
            { 8.0f, 1.0f },
            { 6.0f, 3.0f },
            { 4.0f, 2.0f },
        }
    };
    auto res = sat(a, b);
    ASSERT_FALSE(res.has_value());
}

TEST(PhysicsSATTest, OverlappingCircles)
{
    sat_algorithm sat;
    circle_shape a{
        .center = { 1.f, 1.f },
        .radius = 5.f,
    };
    circle_shape b{
        .center = { 4.f, 1.f },
        .radius = 1.f
    };
    auto res = sat(a, b);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res->axis, (vec2{ 1.f, 0.f }));
    ASSERT_EQ(res->overlap, 2.f);
}

TEST(PhysicsSATTest, NotOverlappingCircles)
{
    sat_algorithm sat;
    circle_shape a{
        .center = { 1.f, 1.f },
        .radius = 2.f,
    };
    circle_shape b{
        .center = { 4.f, 1.f },
        .radius = 1.f
    };
    auto res = sat(a, b);
    ASSERT_FALSE(res.has_value());
}

TEST(PhysicsSATTest, OverlappingBoxWithCircle)
{
    sat_algorithm sat;
    box_shape a{
        .vertices{
            { 0, 0 },
            { 1, 0 },
            { 1, 1 },
            { 0, 1 },
        }
    };
    circle_shape b{
        .center{ 0.7, 0.5 },
        .radius{ 1.f }
    };
    auto res = sat(a, b);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res->axis, (vec2{ 1.f, 0.f }));
}

TEST(PhysicsSATTest, NotOverlappingBoxWithCircle)
{
    sat_algorithm sat;
    box_shape a{
        .vertices{
            { 0, 0 },
            { 1, 0 },
            { 1, 1 },
            { 0, 1 },
        }
    };
    circle_shape b{
        .center{ 1.7, 1.5 },
        .radius{ 0.2f }
    };
    auto res = sat(a, b);
    ASSERT_FALSE(res.has_value());
}
