#include <lemon/threading/scheduler.h>

#include <lemon/core/assert.h>
#include <lemon/core/service_registry.h>

#include <ctime>

namespace lemon {
using namespace std;
size_type thread_local scheduler::threadIndex;
scheduler::scheduler(service_registry&, size_type threadCount)
{
    srand((unsigned int)time(NULL));
    nbWorkers = threadCount;
    workers.reserve(threadCount);
    for(size_type i = 0; i < threadCount; ++i)
    {
        localQueues.emplace_back();
        globalQueues.emplace_back();
        localMtxs.emplace_back(create_owned<mutex>());
        localCvars.emplace_back(create_owned<condition_variable>());
    }
    localMtxs.emplace_back(create_owned<mutex>());
    localCvars.emplace_back(create_owned<condition_variable>());
    for(size_type i = 0; i < threadCount; ++i)
    {
        workers.emplace_back(
            [&](size_type threadIndex) {
                auto& globalQueue      = globalQueues[threadIndex];
                auto& localQueue       = localQueues[threadIndex];
                auto& localMtx         = *localMtxs[threadIndex];
                auto& localCvar        = *localCvars[threadIndex];
                scheduler::threadIndex = threadIndex;

                while(!shouldEnd.load())
                {
                    {
                        lock_guard lck(sleepingMtx);
                        while(!sleepingJobs.empty())
                        {
                            const auto& top = sleepingJobs.top();
                            if(top->wakeTime > chrono::high_resolution_clock::now())
                            {
                                break;
                            }
                            run(top);
                            sleepingJobs.pop();
                        }
                    }

                    job* j = nullptr;
                    while(!shouldEnd.load())
                    {
                        if(localQueue.try_dequeue(j)) break;
                        if(globalQueue.try_dequeue(j)) break;
                        size_type nbTries = nbWorkers - 1;
                        size_type next    = threadIndex;
                        while(j == nullptr && nbTries-- > 0)
                        {
                            next = next >= nbWorkers - 1 ? 0 : next + 1;
                            if(globalQueues[next].try_dequeue(j))
                                goto end_loop;
                        }
                        std::unique_lock lck(localMtx);
                        localCvar.wait(lck);
                    }
                end_loop:
                    if(shouldEnd.load()) break;
                    j->callable();
                    job_finished(j);
                }
            },
            i);
        scheduler::threadIndex = nbWorkers;
    }
}
scheduler::~scheduler()
{
    shouldEnd.store(true);
    for(size_type i = 0; i < nbWorkers; ++i)
    {
        auto& cvar = *localCvars[i];
        auto& mtx  = *localMtxs[i];
        unique_lock lck(mtx);
        cvar.notify_one();
    }
    for(auto&& t : workers)
    {
        t.join();
    }
}
void scheduler::run(job* jobs, size_type count, waitable* sig)
{
    lemon_assert(jobs != nullptr);
    static thread_local size_type lastUsed = std::rand() % nbWorkers;
    if(sig != nullptr)
    {
        sig->counter.fetch_add(count, std::memory_order_relaxed);
        sig->tid = threadIndex;
    }
    for(; count-- > 0; jobs += 1, lastUsed = (lastUsed + 1) % nbWorkers)
    {
        if(lastUsed == threadIndex)
        {
            lastUsed = (lastUsed + 1) % nbWorkers;
        }
        auto& globalQueue = globalQueues[lastUsed];
        auto& localCvar   = *localCvars[lastUsed];
        jobs->wait        = sig;
        globalQueue.enqueue(jobs);
        localCvar.notify_one();
    }
}
void scheduler::run(job* jobs, size_type count, waitable* sig, size_type tid)
{
    lemon_assert(jobs != nullptr);
    lemon_assert(tid < nbWorkers);
    if(sig != nullptr)
    {
        sig->counter.fetch_add(count, std::memory_order_relaxed);
        sig->tid = threadIndex;
    }
    auto& localQueue = localQueues[tid];
    auto& localCvar  = *localCvars[tid];
    for(; count-- > 0; jobs += 1)
    {
        jobs->wait = sig;
        localQueue.enqueue(jobs);
    }
    localCvar.notify_one();
}
void scheduler::run_in(job* jobs, size_type count, const std::chrono::milliseconds& delta)
{
    auto at = chrono::high_resolution_clock::now() + delta;
    run_at(jobs, count, at);
}
void scheduler::run_at(job* jobs, size_type count, const std::chrono::high_resolution_clock::time_point& at)
{
    lemon_assert(jobs != nullptr);
    if(count == 0) return;
    if(at <= chrono::high_resolution_clock::now())
    {
        run(jobs, count);
        return;
    }
    lock_guard lck(sleepingMtx);
    for(; count-- > 0; jobs += 1)
    {
        jobs->wakeTime = at;
        sleepingJobs.push(jobs);
    }
}
void scheduler::wait(waitable* sig)
{
    lemon_assert(sig != nullptr);
    if(sig->counter.load() != 0)
    {
        auto& localMtx  = *localMtxs[threadIndex];
        auto& localCvar = *localCvars[threadIndex];
        std::unique_lock lock(localMtx);
        localCvar.wait(lock, [&] { return sig->counter.load() == 0; });
    }
}
void scheduler::job_finished(job* j)
{
    if(auto sig = j->wait)
    {
        auto& localMtx  = *localMtxs[sig->tid];
        auto& localCvar = *localCvars[sig->tid];
        if(sig->counter.fetch_sub(1) == 1)
        {
            std::lock_guard _(localMtx);
            localCvar.notify_one();
        }
    }
}
size_type scheduler::get_thread_index()
{
    return threadIndex;
}
} // namespace lemon
