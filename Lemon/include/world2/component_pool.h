#pragma once

#include "type_erased_array.h"
#include "world_primitives.h"

#include <array>
#include <unordered_map>
#include <vector>

namespace lemon {
template<std::size_t PageSize = 128>
class component_pool
{
  public:
    component_pool(componentid componentId, std::size_t componentSize);

    void* add_component(entityid entity);
    void* add_component(entityid entity, void* component);
    template<class T, class... Args>
    T& add_component(entityid entity, Args&&... args);
    template<class T>
    T& add_component(entityid entity, const T& component);
    template<class T>
    T& add_component(entityid entity, T&& component);

    void remove_component(entityid entity);
    void remove_entity(entityid entity);

    void* get_component(entityid entity);
    const void* get_component(entityid entity) const;
    template<class T>
    T& get_component(entityid entity);
    template<class T>
    const T& get_component(entityid entity) const;

    bool has_component(entityid entity) const;

    void* get_all();
    const void* const get_all() const;
    std::size_t get_count() const;

  private:
    componentid componentId;
    std::size_t componentSize;
    std::vector<std::array<PageSize, uint32_t>> pages;
    std::vector<entityid> entityArray;
    type_erased_array componentArray;

  private:
    static constexpr std::size_t get_page_index(entityid entity)
    {
        return static_cast<std::size_t>(entity.id) / PageSize;
    }
    static constexpr std::size_t get_entity_index(entityid entity)
    {
        return static_cast<std::size_t>(entity.id) % PageSize;
    }
    void add_page()
    {
        pages.emplace_back();
        pages.back().fill(null_entity);
    }
    auto& entity_for(entityid entity)
    {
        const auto pageid = entity.id / PageSize;
        const auto index  = entity.id % PageSize;

        const auto diff = i64(pageid) - pages.size();
        if(diff >= 0)
        {
            pages.resize(pageid + 1ULL);
            std::for_each(pages.end() - (diff + 1), pages.end(), [](auto& page) { page.fill(null_entity); });
        }
        return pages[pageid][index];
    }

    auto entity_for_or_null(entityid entity)
    {
        const auto pageid = entity.id / PageSize;
        const auto index  = entity.id % PageSize;

        return pageid < pages.size() ? pages[pageid][index] : entityid::null;
    }
};

template<std::size_t PageSize>
component_pool<PageSize>::component_pool(componentid componentId, std::size_t componentSize):
    componentId(componentId), componentSize(componentSize)
{
}

template<std::size_t PageSize>
void* component_pool<PageSize>::add_component(entityid entity)
{
    entity_for(entity) = entityArray.size();
    entityArray.push_back(entity);
    return componentArray.push();
}

template<std::size_t PageSize>
void* component_pool<PageSize>::add_component(entityid entity, void* component)
{
    entity_for(entity) = entityArray.size();
    entityArray.push_back(entity);
    return componentArray.push(component);
}
template<std::size_t PageSize>
template<class T, class... Args>
T& component_pool<PageSize>::add_component(entityid entity, Args&&... args)
{
    entity_for(entity) = entityArray.size();
    entityArray.push_back(entity);
    return componentArray.push<T>(std::forward<Args>(args)...);
}
template<std::size_t PageSize>
template<class T>
T& component_pool<PageSize>::add_component(entityid entity, const T& component)
{
    entity_for(entity) = entityArray.size();
    entityArray.push_back(entity);
    return componentArray.push(component);
}
template<std::size_t PageSize>
template<class T>
T& component_pool<PageSize>::add_component(entityid entity, T&& component)
{
    entity_for(entity) = entityArray.size();
    entityArray.push_back(entity);
    return componentArray.push(std::move(component));
}
template<std::size_t PageSize>
void component_pool<PageSize>::remove_component(entityid entity)
{
    const auto pageIndex   = get_page_index(entity);
    const auto entityIndex = get_entity_index(entity);
    lemon_assert(pageIndex < pages.size());
    auto& page                = pages[pageIndex];
    const auto componentIndex = page[entityIndex];

    const auto last            = entityArray.back();
    const auto lastPageIndex   = get_page_index(last);
    const auto lastEntityIndex = get_entity_index(last);

    page[entityIndex]                     = entityid::null;
    pages[lastPageIndex][lastEntityIndex] = componentIndex;
    entityArray[componentIndex]           = last;
    entityArray.pop_back();
    componentArray.swap_and_discard(componentIndex);
}
template<std::size_t PageSize>
void component_pool<PageSize>::remove_entity(entityid entity)
{
    remove_component(entity);
}
template<std::size_t PageSize>
void* component_pool<PageSize>::get_component(entityid entity)
{
    const auto pageIndex   = get_page_index(entity);
    const auto entityIndex = get_entity_index(entity);
    auto index             = pages[pageIndex][entityIndex];
    return componentArray.at(index);
}
template<std::size_t PageSize>
const void* component_pool<PageSize>::get_component(entityid entity) const
{
    const auto pageIndex   = get_page_index(entity);
    const auto entityIndex = get_entity_index(entity);
    auto index             = pages[pageIndex][entityIndex];
    return componentArray.at(index);
}
template<std::size_t PageSize>
template<class T>
T& component_pool<PageSize>::get_component(entityid entity)
{
    const auto pageIndex   = get_page_index(entity);
    const auto entityIndex = get_entity_index(entity);
    auto index             = pages[pageIndex][entityIndex];
    return *static_cast<T*>(componentArray.at(index));
}
template<std::size_t PageSize>
template<class T>
const T& component_pool<PageSize>::get_component(entityid entity) const
{
    const auto pageIndex   = get_page_index(entity);
    const auto entityIndex = get_entity_index(entity);
    auto index             = pages[pageIndex][entityIndex];
    return *static_cast<T*>(componentArray.at(index));
}
template<std::size_t PageSize>
bool component_pool<PageSize>::has_component(entityid entity) const
{
    const auto pageIndex   = get_page_index(entity);
    const auto entityIndex = get_entity_index(entity);
    return pageIndex < pages.size()
           && pages[pageIndex][entityIndex] != entityid::null;
}
template<std::size_t PageSize>
void* component_pool<PageSize>::get_all()
{
    return componentArray.get();
}
template<std::size_t PageSize>
const void* const component_pool<PageSize>::get_all() const
{
    return componentArray.get();
}
std::size_t component_pool<PageSize>::get_count() const
{
    return entityArray.size();
}
}
