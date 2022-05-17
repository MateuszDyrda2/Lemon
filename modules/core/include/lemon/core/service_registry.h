#pragma once

#include "service.h"

#include <concepts>
#include <unordered_map>

namespace lemon {
class LEMON_PUBLIC service_registry
{
  public:
    using self_type  = service_registry;
    using index_type = size_type;

  public:
    template<class T, class... Args>
    requires std::is_base_of_v<service, T>
    auto register_service(Args&&... args) -> T&;
    template<class T>
    requires std::is_base_of_v<service, T>
    void unregister_service();

    template<class T>
    requires std::is_base_of_v<service, T>
    auto get_service() -> T&;

  private:
    std::unordered_map<index_type, owned<service>> services;

  private:
    template<class T>
    index_type get_index();
    index_type get_index_impl();
};
template<class T, class... Args>
requires std::is_base_of_v<service, T>
inline auto service_registry::register_service(Args&&... args) -> T&
{
    auto index = get_index<T>();
    lemon_assert(services.count(index) == 0);
    auto ret = services.emplace(create_owned<T>(this, std::forward<Args>(args)...));
    return *ret.first;
}
template<class T>
requires std::is_base_of_v<service, T>
inline void service_registry::unregister_service()
{
    auto index = get_index<T>();
    lemon_assert(services.count(index) != 0);
    services.erase(index);
}
template<class T>
requires std::is_base_of_v<service, T>
inline auto service_registry::get_service() -> T&
{
    auto index = get_index<T>();
    lemon_assert(services.count(index));
    return static_cast<T&>(*(services.at(get_index<T>())));
}
template<class T>
inline service_registry::index_type
service_registry::get_index()
{
    static index_type index = get_index_impl();
    return index;
}
inline service_registry::index_type
service_registry::get_index_impl()
{
    static index_type index = 0;
    return index++;
}
} // namespace lemon
