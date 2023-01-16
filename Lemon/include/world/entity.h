/** @file entity.h
 * @brief File with entity definition
 */
#pragma once

#include "core/assert.h"
#include "system.h"

#include <core/defines.h>

namespace lemon {
/** Entity class */
class LEMON_API entity
{
  public:
    /** @brief Creates an entity */
    entity() = default;
    /** @brief Creates an entity
     * @param reg registry pointer
     * @param handle entity handle
     */
    entity(registry* reg, entity_t handle);
    /** @brief Creates an entity
     * @param _scene scene reference
     * @param handle entity handle
     */
    entity(class scene& _scene, entity_t handle);
    /** @brief Emplace component in entity
     * @param ...args component constructor arguments
     */
    template<class T, class... Args>
    decltype(auto) emplace(Args&&... args);
    /** @brief Emplace or replace component in entity
     * @param ...args component constructor arguments
     */
    template<class T, class... Args>
    decltype(auto) emplace_or_replace(Args&&... args);
    /** @brief Change entity component to a newly created
     * @param ...args component constructor arguments
     */
    template<class T, class... Args>
    decltype(auto) change(Args&&... args);
    /** @brief Change entity component with a callable
     * @param callable function editing the component
     */
    template<class T, class F>
    decltype(auto) patch(F&& callable);
    /** @brief Get a component */
    template<class... Args>
    [[nodiscard]] decltype(auto) get();
    /** @brief Get a component */
    template<class... Args>
    [[nodiscard]] decltype(auto) get() const;
    /** @return true if entity has component */
    template<class T>
    [[nodiscard]] bool has() const;
    /** @return true if entity has any of components */
    template<class... Args>
    [[nodiscard]] bool has_any_of() const;
    /** @return true if entity has all of components */
    template<class... Args>
    [[nodiscard]] bool has_all_of() const;
    /** @brief Remove component from entity */
    template<class T>
    void remove_component();
    /** @return entity handle */
    [[nodiscard]] entity_t get_handle() const { return handle; }
    /** @return entity registry */
    [[nodiscard]] registry* get_registry() { return reg; }
    explicit operator bool() const { return !!reg; }
    /** @brief Set entity transform changed */
    void set_dirty();

  private:
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
