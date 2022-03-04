#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/string_id.h>

#include <atomic>

namespace lemon {
/** @brief Stored resource. Keeps a reference count. */
class resource
{
  public:
    using self_type = resource;

    resource(string_id name);
    resource(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;
    resource(self_type&& other) noexcept;
    self_type& operator =(self_type&& other) noexcept;
    virtual ~resource() = default;
    void increment() noexcept;
    u32 decrement() noexcept;

  private:
    std::atomic<u32> _count; ///< reference count of the resource
    string_id name;
};
} // namespace lemon
