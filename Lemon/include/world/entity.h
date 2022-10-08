#pragma once

#include "core/assert.h"
#include "system.h"

#include <core/defines.h>

namespace lemon {
class LEMON_API entity
{
  public:
    entity() = default;
    template<class T, class... Args>
    decltype(auto) emplace(Args&&... args);
    template<class T, class... Args>
    decltype(auto) emplace_or_replace(Args&&... args);
    template<class T, class... Args>
    decltype(auto) change(Args&&... args);
    template<class T, class F>
    decltype(auto) patch(F&& callable);
    template<class... Args>
    [[nodiscard]] decltype(auto) get();
    template<class... Args>
    [[nodiscard]] decltype(auto) get() const;
    template<class T>
    [[nodiscard]] bool has() const;
    template<class... Args>
    [[nodiscard]] bool has_any_of() const;
    template<class... Args>
    [[nodiscard]] bool has_all_of() const;
    template<class T>
    void remove_component();
    [[nodiscard]] entity_t get_handle() const { return handle; }
    [[nodiscard]] registry* get_registry() { return reg; }
    explicit operator bool() const { return !!reg; }

    void set_dirty();

  private:
    friend class scene;
    entity(registry* reg, entity_t handle):
        handle(handle), reg(reg) { }

    entity_t handle{ entt::null };
    registry* reg{ nullptr };
};
template<class T, class... Args>
decltype(auto) entity::emplace(Args&&... args)
{
    lemon_assert(reg);
    return reg->emplace<T>(handle, std::forward<Args>(args)...);
}
template<class T, class... Args>
decltype(auto) entity::emplace_or_replace(Args&&... args)
{
    lemon_assert(reg);
    return reg->emplace_or_replace<T>(handle, std::forward<Args>(args)...);
}
template<class T, class... Args>
decltype(auto) entity::change(Args&&... args)
{
    lemon_assert(reg);
    return reg->replace<T>(handle, std::forward<Args>(args)...);
}
template<class T, class F>
decltype(auto) entity::patch(F&& callable)
{
    lemon_assert(reg);
    return reg->patch<T>(handle, std::forward<F>(callable));
}
template<class... Args>
decltype(auto) entity::get()
{
    lemon_assert(reg);
    return reg->get<Args...>(handle);
}
template<class... Args>
decltype(auto) entity::get() const
{
    lemon_assert(reg);
    return reg->get<Args...>(handle);
}
template<class T>
bool entity::has() const
{
    lemon_assert(reg);
    return reg->any_of<T>(handle);
}
template<class... Args>
bool entity::has_any_of() const
{
    lemon_assert(reg);
    return reg->any_of<Args...>(handle);
}
template<class... Args>
bool entity::has_all_of() const
{
    lemon_assert(reg);
    return reg->all_of<Args...>(handle);
}
template<class T>
void entity::remove_component()
{
    lemon_assert(reg);
    reg->remove<T>(handle);
}
}
