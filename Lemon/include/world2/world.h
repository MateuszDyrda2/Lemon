#pragma once

#include "component_pool.h"

#include <core/lemon_types.h>

#include <atomic>
#include <memory>

namespace lemon {
class world
{
  public:
    static constexpr std::size_t PoolSize = 128;

  public:
    template<class T>
    struct type
    {
        inline static std::size_t id() { return reinterpret_cast<std::size_t>(&type::id); }
    };

    entityid create();
    void destroy(entityid entity);
    void* add(entityid entity, componentid component, std::size_t componentSize);
    bool all(entityid entity, std::initializer_list<componentid> components);
    bool any(entityid entity, std::initializer_list<componentid> components);
    void remove(entityid entity, componentid component);
    void* get(entityid entity, componentid component);
    entityid get_updated(entityid entity) const;
    bool is_alive(entityid entity) const;
    bool is_enabled(entityid entity) const;
    bool enable(entityid entity);
    bool disable(entityid entity);
    bool kill(entityid entity);

    template<class... Args>
    entityid create(Args&&... args);
    template<class T>
    T& add(entityid entity);
    template<class T, class... Args>
    T& add(entityid entity, Args&&... args);
    template<class T>
    T& add(entityid entity, const T& component);
    template<class T>
    T& add(entityid entity, T&& component);
    template<class... Args>
    bool any(entityid entity);
    template<class... Args>
    bool all(entityid entity);
    template<class T>
    void remove(entityid entity);
    template<class T>
    T& get(entityid entity);
    template<class T>
    const T& get(entityid entity) const;

  private:
    std::unordered_map<componentid, component_pool<PoolSize>> pools;
    std::vector<entityid> entities;
    entityid next;

  private:
    entityid get_next();
    void remove_entity(entityid);
};

template<class... Args>
entityid world::create(Args&&... args)
{
}

template<class T>
T& world::add(entityid entity)
{
    return add(entity, type<T>::id(), sizeof(T));
}

template<class T, class... Args>
T& world::add(entityid entity, Args&&... args)
{
    const auto component = type<T>::id();
    if(auto iter = pools.find(component);
       iter != pools.end())
    {
        return iter->second.add_component<T>(entity, std::forward<Args>(args)...);
    }
    else
    {
        auto res = pools.insert(std::make_pair(
            component,
            component_pool(component, sizeof(T)));

        return res.first->second.add_component<T>(entity, std::forward<Args>(args)...);
    }
}
template<class T>
T& world::add(entityid entity, const T& component)
{
    const auto componentId = type<T>::id();
    if(auto iter = pools.find(componentId);
       iter != pools.end())
    {
        return iter->second.add_component<T>(entity, component);
    }
    else
    {
        auto res = pools.insert(std::make_pair(
            componentId,
            component_pool(componentId, sizeof(T)));

        return res.first->second.add_component<T>(entity, component);
    }
}

template<class T>
T& world::add(entityid entity, T&& component)
{
    const auto componentId = type<T>::id();
    if(auto iter = pools.find(componentId);
       iter != pools.end())
    {
        return iter->second.add_component<T>(entity, std::move(component));
    }
    else
    {
        auto res = pools.insert(std::make_pair(
            componentId,
            component_pool(componentId, sizeof(T)));

        return res.first->second.add_component<T>(entity, std::move(component));
    }
}

template<class... Args>
bool world::any(entityid entity)
{
    return any(entity, { type<Args>::id()... });
}

template<class... Args>
bool world::all(entityid entity)
{
    return all(entity, { type<Args>::id()... });
}

template<class T>
void world::remove(entityid entity)
{
    return remove(entity, type<T>::id());
}
template<class T>
T& world::get(entityid entity)
{
    return *static_cast<T*>(get(entity, type<T>::id()));
}
template<class T>
const T& world::get(entityid entity) const
{
    return *static_cast<const T*>(get(entity, type<T>::id()));
}

}
