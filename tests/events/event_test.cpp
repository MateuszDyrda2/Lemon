#include <gtest/gtest.h>

#include <lemon/events/event_bus.h>

#include <thread>

using namespace lemon;
class event_test : public ::testing::Test
{
  protected:
    static void SetUpTestSuite()
    {
        ebus = new event_bus;
    }
    static void TearDownTestSuite()
    {
        delete ebus;
    }
    static inline event_bus* ebus;
};
struct test_event : public event
{
    test_event(int z):
        event(string_id("test_event")), z(z) { }
    int z;
};
class test_listener : public listener
{
  public:
    void on_event(event* e) override
    {
        z += static_cast<test_event*>(e)->z;
    }
    int z = 0;
};
TEST_F(event_test, OneEvent)
{
    test_listener l1;
    ebus->sink(string_id("test_event")) += &l1;
    {
        ebus->fire(new test_event(3));
        ebus->flush();
        ASSERT_EQ(l1.z, 3);
    }
    ebus->sink(string_id("test_event")) -= &l1;
}
TEST_F(event_test, MultipleEvents)
{
    test_listener listeners[10];
    for(auto& l : listeners)
    {
        ebus->sink(string_id("test_event")) += &l;
    }
    {
        ebus->fire(new test_event(5));
        ebus->fire(new test_event(3));
        ebus->flush();
    }
    for(auto& l : listeners)
    {
        ebus->sink(string_id("test_event")) -= &l;
    }
    for(auto& l : listeners)
    {
        EXPECT_EQ(l.z, 8);
    }
}
TEST_F(event_test, MultithreadedEvent)
{
    test_listener l;
    ebus->sink(string_id("test_event")) += &l;
    {
        std::thread threads[100];
        for(auto& t : threads)
        {
            t = std::thread([] { ebus->fire(new test_event(1)); });
        }
        for(auto& t : threads)
            t.join();
        ebus->flush();
    }
    ebus->sink(string_id("test_event")) -= &l;
    ASSERT_EQ(l.z, 100);
}
TEST_F(event_test, MultithreadedListener)
{
    test_listener l[100];
    std::thread threads[100];
    int i = 0;
    for(auto& t : threads)
    {
        t = std::thread([&, i] { ebus->sink(string_id("test_event")) += &l[i]; });
        ++i;
    }
    for(auto& t : threads)
    {
        t.join();
    }
    ebus->fire(new test_event(7));
    ebus->flush();
    i = 0;
    for(auto& t : threads)
    {
        t = std::thread([&, i] { ebus->sink(string_id("test_event")) -= &l[i]; });
        ++i;
    }
    for(auto& t : threads)
    {
        t.join();
    }

    for(auto& ls : l)
    {
        EXPECT_EQ(ls.z, 7);
    }
}
TEST_F(event_test, MultithreadedEventAndListener)
{
    test_listener l[100];
    std::thread threads[100];
    int i = 0;
    for(auto& t : threads)
    {
        t = std::thread([&, i] { ebus->sink(string_id("test_event")) += &l[i]; });
        ++i;
    }
    for(auto& t : threads) t.join();
    for(auto& t : threads)
    {
        t = std::thread([] { ebus->fire(new test_event(1)); });
    }
    for(auto& t : threads) t.join();
    ebus->flush();
    i = 0;
    for(auto& t : threads)
    {
        t = std::thread([&, i] { ebus->sink(string_id("test_event")) -= &l[i]; });
        ++i;
    }
    for(auto& t : threads) t.join();
    for(auto& ls : l)
    {
        EXPECT_EQ(ls.z, 100) << "Not equal";
    }
}
TEST_F(event_test, ImmediateEvent)
{
    test_listener l;
    ebus->sink(string_id("test_event")) += &l;
    ebus->fire_immediate(new test_event(12));
    ASSERT_EQ(l.z, 12);
    ebus->sink(string_id("test_event")) -= &l;
}
TEST_F(event_test, ImmediateEventMultithreaded)
{
    test_listener l[100];
    std::for_each(l, l + 100, [](test_listener& list) { ebus->sink(string_id("test_event")) += &list; });
    std::thread t([] { ebus->fire_immediate(new test_event(12)); });
    t.join();
    std::for_each(l, l + 100, [](test_listener& list) { ebus->sink(string_id("test_event")) -= &list; });

    std::for_each(l, l + 100, [](test_listener& list) { EXPECT_EQ(list.z, 12); });
}