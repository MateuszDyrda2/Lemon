#pragma once

#include "concurrency.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

#include <condition_variable>
#include <functional>
#include <future>
#include <iterator>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace lemon {
struct job
{
    std::atomic_uint32_t nbChilder;
    job* parent;
    job* runAfter;
    std::function<void(void)> function;
};
class job_queue
{
  public:
    size_type size() const;
    void push(job* j);
    void pop(job** j);

  private:
    std::queue<job*> q;
    spinlock lock;
};

class LEMON_PUBLIC job_system
{
  public:
    using future_t  = std::future<void>;
    using promise_t = std::promise<void>;
    struct thread_data
    {
        job_queue globalQueue;
        job_queue localQueue;
        std::mutex lock;
        std::condition_variable cvar;
    };
    template<int I>
    struct execute_on
    {
        static constexpr int value = I;
    };
    using same_thread = execute_on<-1>;
    using main_thread = execute_on<0>;

  public:
    job_system(size_type workerCount);
    ~job_system();

    void run(const std::function<void(void)>& callable);
    template<int I>
    void run(const std::function<void(void)>& callable, execute_on<I>);
    void run_after(const std::function<void(void)>& callable);

    void finish();
    void wait_for_all();

  private:
    size_type workerCount;
    std::vector<std::thread> workers;
    std::vector<thread_data> threadData;
    static thread_local size_type threadIndex;
    static thread_local job* currentJob;

    std::mutex queueLock;
    std::condition_variable queueCVar;
    std::atomic_bool shouldEnd;

  private:
    void on_child_finished(job* j);
    void allocate_job(job** j);
    void destroy_job(job** j);
};
template<int I>
void job_system::run(const std::function<void(void)>& callable, execute_on<I>)
{
    auto index = execute_on<I>::value;
    if constexpr(execute_on<I>::value == same_thread::value)
    {
        index = threadIndex;
    }

    auto newNode      = new job;
    newNode->function = callable;
    threadData[index].localQueue.push(newNode);
    threadData[index].cvar.notify_one();
}
} // namespace lemon
