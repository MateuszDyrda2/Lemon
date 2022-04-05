#include <lemon/threading/concurrency.h>

#include <immintrin.h>

namespace lemon {

void spinlock::lock() noexcept
{
    if(!_lock.exchange(true, std::memory_order_acquire))
    {
        return;
    }
    while(_lock.load(std::memory_order_relaxed))
    {
        _mm_pause();
    }
}
bool spinlock::try_lock() noexcept
{
    return !_lock.load(std::memory_order_relaxed)
           && !_lock.exchange(true, std::memory_order_acquire);
}
void spinlock::unlock() noexcept
{
    _lock.store(false, std::memory_order_release);
}
} // namespace lemon
