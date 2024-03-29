#include <threading/scheduler.h>

#include <ctime>

namespace lemon {
using namespace std;
scheduler::scheduler(std::size_t threadCount)
{
    srand((unsigned int)time(NULL));
    nbWorkers = threadCount;
    workers.reserve(threadCount);
    for(std::size_t i = 0; i < threadCount; ++i)
    {
        localQueues.emplace_back();
        globalQueues.emplace_back();
        localMtxs.emplace_back(std::make_unique<mutex>());
        localCvars.emplace_back(std::make_unique<condition_variable>());
    }
    localMtxs.emplace_back(std::make_unique<mutex>());
    localCvars.emplace_back(std::make_unique<condition_variable>());
    for(std::size_t i = 0; i < threadCount; ++i)
    {
        workers.emplace_back(
            [&](std::size_t threadIndex) {
                auto& globalQueue = globalQueues[threadIndex];
                auto& localQueue  = localQueues[threadIndex];
                auto& localMtx    = *localMtxs[threadIndex];
                auto& localCvar   = *localCvars[threadIndex];
                get_thread_index(threadIndex);

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
                        std::size_t nbTries = nbWorkers - 1;
                        std::size_t next    = threadIndex;
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
        get_thread_index(nbWorkers);
    }
    logger::info("Scheduler created <{}>", threadCount);
}
scheduler::~scheduler()
{
    shouldEnd.store(true);
    for(std::size_t i = 0; i < nbWorkers; ++i)
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
    logger::info("Scheduler destroyed");
}
void scheduler::run(job* jobs, std::size_t count, waitable* sig)
{
    lemon_assert(jobs != nullptr);
    static thread_local std::size_t lastUsed = std::rand() % nbWorkers;
    if(sig != nullptr)
    {
        sig->counter.fetch_add(static_cast<unsigned int>(count), std::memory_order_relaxed);
        sig->tid = get_thread_index();
    }
    for(; count-- > 0; jobs += 1, lastUsed = (lastUsed + 1) % nbWorkers)
    {
        if(lastUsed == get_thread_index())
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
void scheduler::run(job* jobs, std::size_t count, waitable* sig, std::size_t tid)
{
    lemon_assert(jobs != nullptr);
    lemon_assert(tid < nbWorkers);
    if(sig != nullptr)
    {
        sig->counter.fetch_add(static_cast<unsigned int>(count), std::memory_order_relaxed);
        sig->tid = get_thread_index();
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
void scheduler::run_in(job* jobs, std::size_t count, const std::chrono::milliseconds& delta)
{
    auto at = chrono::high_resolution_clock::now() + delta;
    run_at(jobs, count, at);
}
void scheduler::run_at(job* jobs, std::size_t count,
                       const std::chrono::high_resolution_clock::time_point& at)
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
        auto threadIndex = get_thread_index();
        auto& localMtx   = *localMtxs[threadIndex];
        auto& localCvar  = *localCvars[threadIndex];
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
std::size_t scheduler::get_thread_index(std::size_t init)
{
    static thread_local std::size_t idx = init;
    return idx;
}
} // namespace lemon
