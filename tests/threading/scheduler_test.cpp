#include <gtest/gtest.h>

#include <lemon/threading/scheduler.h>

#include <iostream>
#include <thread>

using namespace lemon;
class scheduler_test : public ::testing::Test
{
  protected:
    static void SetUpTestSuite()
    {
        scheduler::create(std::thread::hardware_concurrency() - 1);
    }
    static void TearDownTestSuite()
    {
        scheduler::dispose();
    }
};

TEST_F(scheduler_test, RunOneJob)
{
    int w = 2;
    waitable s;
    job j = [&] {
        w += 1;
    };
    scheduler::run(&j, 1, &s);
    scheduler::wait(&s);
    ASSERT_EQ(w, 3);
}
TEST_F(scheduler_test, RunMultiple)
{
    job jobs[4];
    int x = 1, y = 2, z = 3, w = 4;
    jobs[0] = [&] {
        ++x;
    };
    jobs[1] = [&] {
        ++y;
    };
    jobs[2] = [&] {
        ++z;
    };
    jobs[3] = [&] {
        ++w;
    };
    waitable sig;
    scheduler::run(jobs, 4, &sig);
    scheduler::wait(&sig);
    ASSERT_EQ(x, 2) << "not the same value";
    ASSERT_EQ(y, 3) << "not the same value";
    ASSERT_EQ(z, 4) << "not the same value";
    ASSERT_EQ(w, 5) << "not the same value";
}
TEST_F(scheduler_test, RunConsecutive)
{
    job jobs[4];
    int z   = 3;
    jobs[0] = [&] {
        z *= 2;
    };
    jobs[1] = [&] {
        z += 2;
    };
    jobs[2] = [&] {
        z *= 3;
    };
    jobs[3] = [&] {
        z -= 2;
    };
    waitable sig;
    scheduler::run(&jobs[0], 1, &sig);
    scheduler::wait(&sig);
    scheduler::run(&jobs[1], 1, &sig);
    scheduler::wait(&sig);
    scheduler::run(&jobs[2], 1, &sig);
    scheduler::wait(&sig);
    scheduler::run(&jobs[3], 1, &sig);
    scheduler::wait(&sig);
    ASSERT_EQ(z, 22) << "not equal";
}
TEST_F(scheduler_test, ParrarelForBig)
{
    int arr[256];
    for(size_t i = 0; i < 256; ++i)
    {
        arr[i] = i;
    }
    scheduler::for_each(arr, arr + 256, [](int& i) {
        i = 3;
    });
    for(auto& a : arr)
    {
        EXPECT_EQ(a, 3) << a << " and " << 3;
    }
}
TEST_F(scheduler_test, ParrarelForSmall)
{
    int arr[3] = { 0, 1, 2 };
    scheduler::for_each(arr, arr + 3, [](int& i) {
        i += 1;
    });
    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[1], 2);
    ASSERT_EQ(arr[2], 3);
}
TEST_F(scheduler_test, NestedJobs)
{
    int z = 0;
    job j;
    waitable sig;
    j.callable = [&] {
        job w;
        waitable s;
        z += 5;
        w.callable = [&] {
            z += 4;
        };
        scheduler::run(&w, 1, &s);
        scheduler::wait(&s);
    };
    scheduler::run(&j, 1, &sig);
    scheduler::wait(&sig);
    ASSERT_EQ(z, 9);
}
TEST_F(scheduler_test, ChoosenThread)
{
    job j;
    size_type threadIndex;
    j.callable = [&] {
        threadIndex = scheduler::get_thread_index();
    };
    waitable sig;
    scheduler::run(&j, 1, &sig, 2);
    scheduler::wait(&sig);
    ASSERT_EQ(threadIndex, 2);
}
TEST_F(scheduler_test, MultipleJobs)
{
    job jobs[5];
    bool run[5] = {};
    jobs[0]     = [&] {
        run[0] = true;
    };
    jobs[1] = [&] {
        run[1] = true;
    };
    jobs[2] = [&] {
        run[2] = true;
    };
    jobs[3] = [&] {
        run[3] = true;
    };
    jobs[4] = [&] {
        run[4] = true;
    };
    waitable w;
    scheduler::run(jobs, 1, &w);
    scheduler::run(jobs + 1, 1, &w);
    scheduler::run(jobs + 2, 1, &w);
    scheduler::run(jobs + 3, 1, &w);
    scheduler::run(jobs + 4, 1, &w);
    scheduler::wait(&w);
    for(auto& r : run)
        EXPECT_TRUE(r);
}