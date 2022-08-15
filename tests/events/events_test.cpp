#include <gtest/gtest.h>

#include <events/events.h>

using namespace lemon;
struct test_event : public event_args
{
    test_event(int arg):
        arg(arg) { }
    int arg;
};
TEST(EventQueueTest, SimpleEvent)
{
    event_queue queue;
    {
        int init = 0;
        auto e   = queue.event("test"_hs).subscribe([&](event_args* t) { init = ((test_event*)t)->arg; });
        queue.event("test"_hs).fire(new test_event(1));
        queue.process();
        ASSERT_EQ(init, 1);
    }
}
TEST(EventQueueTest, EmptyFire)
{
    event_queue queue;
    {
        int init = 0;
        auto e   = queue.event("test"_hs).subscribe([&](event_args* t) { init = ((test_event*)t)->arg; });
        queue.event("test"_hs).fire(new test_event(1));
        ASSERT_EQ(init, 0);
    }
}
TEST(EventQueueTest, EventDestruction)
{
    event_queue queue;
    {
        int init = 0;
        {
            auto e = queue.event("test"_hs).subscribe([&](event_args* t) { init = ((test_event*)t)->arg; });
        }
        queue.event("test"_hs).fire(new test_event(1));
        queue.process();
        ASSERT_EQ(init, 0);
    }
}
TEST(EventQueueTest, MultipleEvents)
{
    event_queue queue;
    {
        int init = 0;
        auto e1  = queue["test"_hs].subscribe([&](event_args* t) { init += ((test_event*)t)->arg; });
        queue["test"_hs].fire(new test_event(1));
        queue["test"_hs].fire(new test_event(2));
        queue["test"_hs].fire(new test_event(3));
        queue.process();
        ASSERT_EQ(init, 6);
    }
}
TEST(EventQueueTest, MultipleListeners)
{
    event_queue queue;
    {
        int init = 0;
        auto e1  = queue["test"_hs] += [&](event_args* t) {
            init += ((test_event*)t)->arg;
        };
        auto e2 = queue["test"_hs] += [&](event_args* t) {
            init += ((test_event*)t)->arg;
        };
        auto e3 = queue["test"_hs] += [&](event_args* t) {
            init += ((test_event*)t)->arg;
        };
        auto e4 = queue["test"_hs] += [&](event_args* t) {
            init += ((test_event*)t)->arg;
        };
        queue["test"_hs].fire(new test_event(1));
        queue.process();
        ASSERT_EQ(init, 4);
    }
}
static int t_1 = 0;
void f(event_args* args)
{
    t_1 += ((test_event*)args)->arg;
}
TEST(EventQueueTest, FreeFunction)
{
    event_queue queue;
    {
        auto e = queue["test"_hs] += f;
        queue["test"_hs].fire(new test_event(1));
        queue.process();
        ASSERT_EQ(t_1, 1);
    }
}
class t_2
{
  public:
    void f(event_args* args)
    {
        val += ((test_event*)args)->arg;
    }
    int val = 0;
};
TEST(EventQueueTest, Method)
{
    using namespace std::placeholders;
    event_queue queue;
    {
        t_2 t;
        auto ff = std::bind(&t_2::f, &t, _1);
        auto e  = queue["test"_hs].subscribe(ff);
        queue["test"_hs].fire(new test_event(1));
        queue.process();
        ASSERT_EQ(t.val, 1);
    }
}
static int t_3val = 0;
class t_3
{
  public:
    void operator()(event_args* args)
    {
        t_3val += ((test_event*)args)->arg;
    }
};
TEST(EventQueueTest, Functor)
{
    event_queue queue;
    {
        t_3 t;
        auto e = queue["test"_hs].subscribe(t);
        queue["test"_hs].fire(new test_event(1));
        queue.process();
        ASSERT_EQ(t_3val, 1);
    }
}
TEST(EventQueueTest, Lambda)
{
    event_queue queue;
    {
        int a  = 0;
        auto e = queue["test"_hs] += [&](event_args* args) {
            a += ((test_event*)args)->arg;
        };
        queue["test"_hs].fire(new test_event(1));
        queue.process();
        ASSERT_EQ(a, 1);
    }
}
TEST(EventQueueTest, DifferentEvents)
{
    event_queue queue;
    {
        int a  = 0;
        auto l = [&](event_args* args) {
            a += ((test_event*)args)->arg;
        };
        auto e1 = queue["test1"_hs] += l;
        auto e2 = queue["test2"_hs] += l;
        auto e3 = queue["test3"_hs] += l;
        auto e4 = queue["test4"_hs] += l;

        queue["test1"_hs].fire(new test_event(1));
        queue["test2"_hs].fire(new test_event(2));
        queue["test3"_hs].fire(new test_event(3));
        queue["test4"_hs].fire(new test_event(4));

        queue.process();
        ASSERT_EQ(a, 10);
    }
}
