#pragma once

#include <core/defines.h>
#include <core/hash_string.h>
#include <core/lemon_types.h>

#include <atomic>

namespace lemon {
/** @brief Stored resource. Keeps a reference count. */
class LEMON_API resource
{
  public:
    using self_type = resource;

    resource(hash_str name);
    resource(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;
    resource(self_type&& other) noexcept;
    self_type& operator =(self_type&& other) noexcept;
    virtual ~resource() = default;
    void increment() noexcept;
    u32 decrement() noexcept;

  private:
    std::atomic<u32> _count; ///< reference count of the resource
    hash_str name;
};
} // namespace lemon
