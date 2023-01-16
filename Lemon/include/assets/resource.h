/** @file resource.h
 * @brief file with resource base class
 */
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

    /** @brief Creates a resource
     * @param nameid id of the created resource
     */
    resource(hashstr nameid);
    resource(const self_type&)             = delete;
    self_type& operator=(const self_type&) = delete;
    resource(self_type&& other) noexcept;
    self_type& operator=(self_type&& other) noexcept;
    virtual ~resource() = default;
    /** @brief increments the stored atomic count of the resource */
    void increment() noexcept;
    /** @brief decrements the stored atomic count of the resource
     * @return previous value
     */
    u32 decrement() noexcept;

  private:
    std::atomic<u32> _count; ///< reference count of the resource
    hashstr nameid;
};
} // namespace lemon
