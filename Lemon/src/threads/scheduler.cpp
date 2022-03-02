#include <lemon/threads/scheduler.h>

namespace lemon {
ptr<scheduler> scheduler::system = nullptr;
scheduler::scheduler(size_type workerCount):
    finish{ false }
{
    LEMON_ASSERT(!system);
    system = this;
    while(workerCount-- > 0)
    {
        workers.emplace_back(
            [this]() {
                while(!finish.load())
                {
                    callable_type task;
                    {
                        std::unique_lock<mutex_type> lck(mtx);
                        while(!finish && (tasks.empty() || tasks.top().time < clock_type::now()))
                            cvar.wait(lck);
                        if(finish.load()) break;
                        task = std::move(tasks.top().task);
                        tasks.pop();
                    }
                    task();
                }
            });
    }
    LOG_MESSAGE("Scheduler created");
}
scheduler::~scheduler()
{
    finish.store(true);
    cvar.notify_all();
    for(auto& w : workers)
        w.join();
    LOG_MESSAGE("Scheduler destroyed");
}
void scheduler::run(const callable_type& callable)
{
    run_at(callable, clock_type::now());
}
void scheduler::run_after(const callable_type& callable, const duration& after)
{
    run_at(callable, clock_type::now() + after);
}
void scheduler::run_at(const callable_type& callable, const time_point& at)
{
    scheduler* s = system;
    s->mtx.lock();
    s->tasks.push(function{ callable, at });
    s->mtx.unlock();
    s->cvar.notify_one();
}
} // namespace lemon
