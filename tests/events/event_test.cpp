#include <gtest/gtest.h>

#include <lemon/events/event.h>

using namespace lemon;
TEST(SendingEvents, OneEvent)
{
    struct t_event : public event_args
    {
        t_event(int z):
            z(z) { }
        int z;
    };
    event testEvent;
    int w = 0;
    testEvent.register_observer([&](event_args* e) {
        w += static_cast<t_event*>(e)->z;
    },
                                string_id("ob"));
    testEvent.notify(new t_event(4));
    ASSERT_EQ(w, 4);
}
