#pragma once

#include "component.h"
#include "object.h"

#include <memory>
#include <vector>

namespace river {
class entity : public object
{
  public:
    entity(std::string const& name);
    entity(string_id id);
    ~entity();

    void on_enable();
    void update(float deltaTime);
    void fixed_update(float fixedDelta);
    void late_update(float deltaTime);
    void on_disable();

    template<class T>
    std::shared_ptr<component>& add_component()
    {
        size_type index = components.size();
        auto& ret       = components.emplace_back(new T(id));
        componentIndexes.push_back(index);
        get_type_idx<T>();
        return auto;
    }
    template<class T>
    std::shared_ptr<T>& get_component()
    {
        return static_pointer_cast<T>((components.at(componentIndexes.at(get_type_idx<T>()))));
    }

    void set_active(bool value);

  private:
    std::vector<std::shared_ptr<component>> components;
    std::vector<std::shared_ptr<component>> toDestroy;
    std::vector<size_type> componentIndexes;
    bool isActive;

  private:
    void destroy_this() override;
    object* clone_this() override;
    size_type get_index()
    {
        static size_type idx = 0;
        return idx++;
    }
    template<class T>
    size_type get_type_idx()
    {
        static size_type typeIdx = get_index();
        return typeIdx;
    }
};
} // namespace river
