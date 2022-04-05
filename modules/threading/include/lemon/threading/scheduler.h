#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

#include <moodycamel/concurrentqueue.h>

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace lemon {
struct LEMON_PUBLIC waitable
{
    std::atomic_uint32_t counter{};
    size_type tid{};
};
struct LEMON_PUBLIC job
{
    std::function<void(void)> callable;
    waitable* wait{};
    job() = default;
    template<class F>
    job(F&& f):
        callable(f) { }
};
class LEMON_PUBLIC scheduler
{
  public:
    template<class T>
    using concurrent_queue = moodycamel::ConcurrentQueue<T>;

  public:
    /** @brief Creates a scheduler
     * @param threadCount number of workers to create
     */
    static void create(size_type threadCount);
    /** @brief Destroys the scheduler
     */
    static void dispose();
    /** @brief Run jobs in another thread
     * @param jobs jobs that are to be scheduled
     * @param sig object to be waited on
     */
    static void run(job* jobs, size_type count = 1, waitable* sig = nullptr);
    /** @brief Run jobs on a choosen thread
     * @param jobs jobs that are to be scheduled
     * @param sig object to be waited on
     * @param threadIndex index of the thread
     */
    static void run(job* jobs, size_type count, waitable* sig, size_type threadIndex);
    /** @brief Wait untill the results of the jobs waited on
     * become available
     * @param sig waitable object
     */
    static void wait(waitable* sig);
    /** @returns index of the calling thread */
    static size_type get_thread_index();
    /** @brief Executes an unary function on each element in range
     * on multiple workers
     * @param beg iterator to the beggining of the range
     * @param end iterator to the end of the range
     * @param callable a callable object to be executed for each element
     */
    template<class Iter, class F>
    static void for_each(Iter beg, Iter end, F callable);

  private:
    static std::vector<std::thread> workers;
    static size_type nbWorkers;
    static std::vector<concurrent_queue<job*>> localQueues;
    static std::vector<concurrent_queue<job*>> globalQueues;
    static std::vector<owned<std::mutex>> localMtxs;
    static std::vector<owned<std::condition_variable>> localCvars;

    static std::atomic_bool shouldEnd;
    static thread_local size_type threadIndex;

  private:
    static void job_finished(job* j);
};
template<class Iter, class F>
void scheduler::for_each(Iter beg, Iter end, F callable)
{
    size_type _size = std::distance(beg, end);
    if(_size < nbWorkers)
    {
        for(; beg != end; ++beg)
        {
            callable(*beg);
        }
    }
    else
    {
        auto step = _size / nbWorkers;
        job jobs[nbWorkers - 1];
        for(size_type i = 0; i < nbWorkers - 1; ++i)
        {
            jobs[i].callable = [&, it = beg]() mutable {
                for(size_type j = 0; j < step; ++j, ++it)
                {
                    callable(*it);
                }
            };
            std::advance(beg, step);
        }
        lemon::waitable sig;
        run(jobs, nbWorkers - 1, &sig);
        for(; beg != end; ++beg)
        {
            callable(*beg);
        }
        wait(&sig);
    }
}
} // lemon