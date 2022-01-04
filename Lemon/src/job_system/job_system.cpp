#include <lemon/job_system/job_system.h>

namespace lemon {
job_system::job_system(size_type workerCount):
    finish{ false }
{
    while(workerCount-- > 0)
    {
        workers.emplace_back(
            [this]() {
                while(!finish.load())
                {
                    owned<task_type> task;
                    {
                        std::unique_lock<mutex_type> lck(mtx);
                        cvar.wait(lck, [this]() { return !tasks.empty(); });
                        if(finish.load()) break;
                        task = std::move(tasks.front());
                        tasks.pop_front();
                    }
                    (*task)();
                }
            });
    }
}
job_system::~job_system()
{
    finish.store(true);
    for(auto& w : workers)
        w.join();
}
} // namespace lemon
