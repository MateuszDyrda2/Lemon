#pragma once

#include <atomic>

namespace lemon {
struct spinlock
{
    std::atomic_bool _lock{ 0 };

    void lock() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;
};

} // namespace lemon
