#pragma once

#include <lemon/core/basic_types.h>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

namespace lemon {
class job_system
{
  public:
    using worker_type             = std::thread;
    using worker_container_type   = std::vector<worker_type>;
    using task_type               = std::function<void()>;
    using task_container_type     = std::deque<owned<task_type>>;
    using mutex_type              = std::mutex;
    using condition_variable_type = std::condition_variable;
    using bool_flag_type          = std::atomic_bool;
    template<class R>
    using future_type = std::future<R>;

  public:
    job_system(size_type workerCount);
    ~job_system();

    template<class F>
    void schedule(F&& callable);
    template<class F, class... Args>
    void schedule(F&& callable, Args&&... args);
    template<class F>
    future_type<std::invoke_result_t<std::decay_t<F>>>
    schedule_r(F&& callable);
    template<class F, class... Args>
    future_type<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
    schedule_r(F&& callable, Args&&... args);

  private:
    worker_container_type workers;
    task_container_type tasks;
    mutex_type mtx;
    condition_variable_type cvar;
    bool_flag_type finish;
};
template<class F>
void job_system::schedule(F&& callable)
{
    auto task = create_owned<task_type>(std::forward<F>(callable));
    mtx.lock();
    tasks.push_back(std::move(task));
    mtx.unlock();
    cvar.notify_one();
}
template<class F, class... Args>
void job_system::schedule(F&& callable, Args&&... args)
{
    auto task = create_owned<task_type>(
        [callable = std::forward<F>(callable), ... args = std::forward<Args>(args)]() {
            std::forward<F>(callable)(std::forward<Args>(args)...);
        });
    mtx.lock();
    tasks.push_back(std::move(task));
    mtx.unlock();
    cvar.notify_one();
}
template<class F>
typename job_system::future_type<std::invoke_result_t<std::decay_t<F>>>
job_system::schedule_r(F&& callable)
{
    using return_type = std::invoke_result_t<std::decay_t<F>>;
    auto task         = create_owned<task_type>(std::forward<F>(callable));
    auto returnFuture = task.get_future();
    mtx.lock();
    tasks.push_back(std::move(task));
    mtx.unlock();
    cvar.notify_one();

    return returnFuture;
}
template<class F, class... Args>
typename job_system::future_type<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
job_system::schedule_r(F&& callable, Args&&... args)
{
    using return_type = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;
    std::promise<return_type> returnValue;
    future_type<return_type> returnFuture = returnValue.get_future();
    auto task                             = create_owned<task_type>(
        [returnValue = std::move(returnValue), callable = std::forward<F>(callable),
         ... args = std::forward<Args>(args)]() {
            returnValue.set_value(std::invoke(std::forward<F>(callable), std::forward<Args>(args)...));
        });

    mtx.lock();
    tasks.push_back(std::move(task));
    mtx.unlock();
    cvar.notify_one();

    return returnFuture;
}
} // namespace lemon
