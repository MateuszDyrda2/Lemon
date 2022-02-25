#pragma once

#include <lemon/core/basic_types.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace lemon {
/** Class for scheduling multithreaded jobs */
class LEMON_PUBLIC scheduler
{
  private:
    struct function
    {
        std::function<void()> task;
        std::chrono::high_resolution_clock::time_point time;

        bool operator<(const function& other) const
        {
            return time > other.time;
        }
    };

  public:
    using worker_type             = std::thread;                        ///< type of working thread
    using worker_container_type   = std::vector<worker_type>;           ///< thread pool
    using task_container_type     = std::priority_queue<function>;      ///< queue of tasks
    using callable_type           = std::function<void()>;              ///< a callable function
    using clock_type              = std::chrono::high_resolution_clock; ///< clock type
    using time_point              = clock_type::time_point;             ///< time point type
    using duration                = clock_type::duration;               ///< duration type
    using mutex_type              = std::mutex;                         ///< mutex type
    using condition_variable_type = std::condition_variable;            ///< condition variable type
    using bool_flag_type          = std::atomic_bool;                   ///< type of a bool flag

  public:
    /** @brief Creates a scheduler with a specified number of worker threads running */
    scheduler(size_type workerCount);
    ~scheduler();

    /** @brief Schedules a task to run now
     * @param callable a callable object / function / method to be scheduled
     */
    static void run(const callable_type& callable);
    /** @brief Schedules a task to run after a specified interval
     * @param callable a callable object / function / method to be scheduled
     * @param after time after the callable should be run
     */
    static void run_after(const callable_type& callable, const duration& after);
    /** @brief Schedules a task to run at a set time in the future
     * @param callable a callable object / function / method to be scheduled
     * @param at time in the future the callable should run at
     */
    static void run_at(const callable_type& callable, const time_point& at);

  private:
    worker_container_type workers;
    task_container_type tasks;
    mutex_type mtx;
    condition_variable_type cvar;
    bool_flag_type finish;
    static ptr<scheduler> system;
};
} // namespace lemon
