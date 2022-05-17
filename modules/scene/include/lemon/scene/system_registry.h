#pragma once

#include <lemon/core/assert.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/system.h>

#include <concepts>
#include <unordered_map>

namespace lemon {
class LEMON_PUBLIC system_registry
{
  public:
    template<class T, class... Args>
    requires std::is_base_of_v<system, T>
    auto register_system(Args&&... args) -> ptr<T>;
    template<class T>
    requires std::is_base_of_v<system, T>
    void unregister_system();

    template<class T>
    requires std::is_base_of_v<system, T>
    auto get_system() -> ptr<T>;

  private:
    std::unordered_map<size_type, owned<system>> systems;

  private:
    template<class T>
    size_type get_index();
    size_type get_index_impl();
};
template<class T, class... Args>
requires std::is_base_of_v<system, T>
auto system_registry::register_system(Args&&... args) -> ptr<T>
{
    auto index = get_index<T>();
    assert_lemon(systems.count(index) == 0);
    auto ret = systems.emplace(create_owned<T>(std::forward<Args>(args)...));
    return ret.first->get();
}
template<class T>
requires std::is_base_of_v<system, T>
void system_registry::unregister_system()
{
    auto index = get_index<T>();
    assert_lemon(systems.count(index) != 0);
    systems.erase(index);
}
template<class T>
requires std::is_base_of_v<system, T>
auto system_registry::get_system() -> ptr<T>
{
    return static_cast<ptr<T>>(systems.at(get_index<T>()));
}
template<class T>
size_type system_registry::get_index()
{
    static size_type index = get_index_impl();
    return index;
}
size_type system_registry::get_index_impl()
{
    static size_type index = 0;
    return index++;
}
} // namespace lemon
