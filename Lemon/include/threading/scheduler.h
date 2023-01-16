/** @file scheduler.h
 * @brief Scheduler implementation
 */
#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>

#include <concurrentqueue.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace lemon {
struct LEMON_API waitable
{
    std::atomic_uint32_t counter{};
    std::size_t tid{};
};
struct LEMON_API job
{
    std::function<void(void)> callable;
    waitable* wait{};
    std::chrono::high_resolution_clock::time_point wakeTime{};
    job() = default;
    template<class F>
    job(F&& f):
        callable(f) { }

    bool operator<(const job& other)
    {
        return wakeTime < other.wakeTime;
    }
};
/** Scheduler implementation */
class LEMON_API scheduler
{
  public:
    template<class T>
    using concurrent_queue = moodycamel::ConcurrentQueue<T>;

  public:
    /** @brief Creates a scheduler
     * @param threadCount number of workers to create
     */
    scheduler(std::size_t threadCount);
    ~scheduler();
    /** @brief Run jobs in another thread
     * @param jobs jobs that are to be scheduled
     * @param sig object to be waited on
     */
    void run(job* jobs, std::size_t count = 1, waitable* sig = nullptr);
    /** @brief Run jobs on a choosen thread
     * @param jobs jobs that are to be scheduled
     * @param sig object to be waited on
     * @param threadIndex index of the thread
     */
    void run(job* jobs, std::size_t count, waitable* sig, std::size_t threadIndex);
    /** @brief Wait untill the results of the jobs waited on
     * become available
     * @param sig waitable object
     */
    void wait(waitable* sig);
    /** @brief Run function in seconds
     * @param jobs jobs that are to be scheduled
     * @param count number of jobs
     * @param delta time offset
     */
    void run_in(job* jobs, std::size_t count, const std::chrono::milliseconds& delta);
    /** @brief Run function at time
     * @param jobs jobs that are to be scheduled
     * @param count number of jobs
     * @param at scheduled time
     */
    void run_at(job* jobs, std::size_t count, const std::chrono::high_resolution_clock::time_point& at);
    /** @returns index of the calling thread */
    std::size_t get_thread_index(std::size_t init = 0);
    /** @brief Executes an unary function on each element in range
     * on multiple workers
     * @param beg iterator to the beggining of the range
     * @param end iterator to the end of the range
     * @param callable a callable object to be executed for each element
     */
    template<class Iter, class F>
    void for_each(Iter beg, Iter end, F callable);

  private:
    std::vector<std::thread> workers;
    std::size_t nbWorkers;

    std::priority_queue<job*> sleepingJobs;
    std::mutex sleepingMtx;
    std::vector<concurrent_queue<job*>> localQueues;
    std::vector<concurrent_queue<job*>> globalQueues;
    std::vector<std::unique_ptr<std::mutex>> localMtxs;
    std::vector<std::unique_ptr<std::condition_variable>> localCvars;

    std::atomic_bool shouldEnd;

  private:
    void job_finished(job* j);
};
template<class Iter, class F>
void scheduler::for_each(Iter beg, Iter end, F callable)
{
    std::size_t _size = std::distance(beg, end);
    if (_size < nbWorkers)
    {
        for (; beg != end; ++beg)
        {
            callable(*beg);
        }
    }
    else
    {
        auto step = _size / nbWorkers;
        std::vector<job> jobs(nbWorkers - 1);
        for (std::size_t i = 0; i < nbWorkers - 1; ++i)
        {
            jobs[i].callable = [&, it = beg]() mutable {
                for (std::size_t j = 0; j < step; ++j, ++it)
                {
                    callable(*it);
                }
            };
            std::advance(beg, step);
        }
        lemon::waitable sig;
        run(jobs.data(), nbWorkers - 1, &sig);
        for (; beg != end; ++beg)
        {
            callable(*beg);
        }
        wait(&sig);
    }
}
} // lemon