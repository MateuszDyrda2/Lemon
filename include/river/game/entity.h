#pragma once

#include "component.h"
#include "object.h"
#include <river/basic_components/transform.h>

#include <memory>
#include <vector>

namespace river {
class scene;
class entity : public object
{
  public:
    entity(const std::string& name, scene* this_scene);
    entity(string_id id, scene* this_scene);
    ~entity();

    void awake_components();
    void start();
    void destroy_pending();

    template<class T>
    T& add_component(bool active = true);
    template<class T>
    T& get_component();

    bool is_active() const { return !!active; }
    bool is_enabled() const { return !!enabled; }

    void enable();
    void disable();

    void on_enable();
    void on_disable();

    void set_active(bool value);
    transform& get_transform() { return *_transform; };
    const transform& get_transform() const { return *_transform; }

  private:
    std::vector<std::unique_ptr<component>> components;
    std::vector<size_type> toDestroy;
    std::vector<size_type> componentIndexes;
    std::unique_ptr<transform> _transform;
    u8 active : 1;
    u8 enabled : 1;
    scene* _scene;

  private:
    void destroy_this();
    object* clone_this();
    size_type get_index();
    template<class T>
    size_type get_type_idx();
    friend class component;
    void destroy_component(size_type index);
};
template<class T>
T& entity::add_component(bool active)
{
    size_type index = components.size();
    auto idx        = get_type_idx<T>();
    auto& ret       = components.emplace_back(new T(this, idx, true));
    componentIndexes.push_back(index);
    return *std::static_pointer_cast<T>(ret);
}
template<class T>
T& entity::get_component()
{
    return *((components.at(componentIndexes.at(get_type_idx<T>()))).get());
}
template<class T>
size_type entity::get_type_idx()
{
    static size_type typeIdx = get_index();
    return typeIdx;
}
} // namespace river