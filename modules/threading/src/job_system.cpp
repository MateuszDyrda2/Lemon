#include <lemon/threading/job_system.h>

#include <lemon/core/assert.h>

namespace lemon {
using namespace std;
size_type job_queue::size() const
{
    std::lock_guard lck(lock);
    return q.size();
}
void job_queue::push(job* j)
{
    std::lock_guard lck(lock);
    q.push(j);
}
void job_queue::pop(job** j)
{
    lock_guard lck(lock);
    *j = q.front();
    q.pop();
}
job_system::job_system(size_type workerCount)
{
    LEMON_ASSERT(workerCount >= 4);
    auto workerJob = [this](size_type index) {
        auto& globalQueue = threadData[index].globalQueue;
        auto& localQueue  = threadData[index].localQueue;
        auto& localLock   = threadData[index].lock;
        auto& localCVar   = threadData[index].cvar;
        threadIndex       = index;
        while(!shouldEnd.load())
        {
            job* j = nullptr;
            localQueue.pop(&j);
            if(!j)
            {
                globalQueue.pop(&j);
            }
            // try to steal
            size_type tries = this->workerCount - 1;
            size_type next  = index;
            while(j == nullptr && --tries > 0)
            {
                next = ++next >= this->workerCount ? 0 : next;
                threadData[next].globalQueue.pop(&j);
            }

            if(j != nullptr)
            {
                currentJob = j;
                j->function();
                on_child_finished(j);
                destroy_job(&j);
            }
            else
            {
                std::unique_lock lck(localLock);
                localCVar.wait(
                    lck, [&] {
                        return localQueue.size() != 0
                               || globalQueue.size() != 0;
                    });
            }
        }
    };
    for(size_type i = 1; i < workerCount; ++i)
    {
        workers.emplace_back(workerJob, i);
    }
    workerJob(0);
}
job_system::~job_system()
{
}
void job_system::run(const std::function<void(void)>& callable)
{
    static thread_local size_type lastUsed = std::rand() % workerCount;

    auto newNode      = new job;
    newNode->function = callable;
    newNode->parent   = currentJob;
    ++currentJob->nbChilder;
    threadData[lastUsed].globalQueue.push(newNode);
    threadData[lastUsed].cvar.notify_one();

    lastUsed = ++lastUsed >= workerCount ? 0 : lastUsed;
}
void job_system::run_after(const std::function<void(void)>& callable)
{
    auto j               = new job;
    j->function          = callable;
    currentJob->runAfter = j;
}
void job_system::finish()
{
    shouldEnd = true;
    queueCVar.notify_all();
}
void job_system::wait_for_all()
{
    for(auto& w : workers)
    {
        if(w.joinable()) w.join();
    }
}
void job_system::on_child_finished(job* j)
{
    if(j->nbChilder.fetch_sub(1) == 1)
    {
        if(auto runAfter = j->runAfter)
        {
            if(auto parent = j->parent)
            {
                runAfter->parent = parent;
                parent->nbChilder.fetch_add(1);
            }
            run(runAfter->function);
        }
        if(auto parent = j->parent)
        {
            on_child_finished(parent);
        }
    }
}
void job_system::allocate_job(job** j)
{
    *j              = new job;
    (*j)->nbChilder = 1;
    (*j)->parent    = nullptr;
    (*j)->runAfter  = nullptr;
}
void job_system::destroy_job(job** j)
{
    delete(*j);
    *j = nullptr;
}
} // namespace lemon
