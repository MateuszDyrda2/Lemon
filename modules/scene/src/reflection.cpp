#include <lemon/scene/reflection.h>

namespace lemon {
std::unordered_map<std::string, detail::_component_def>&
reflection::get_registeredComponents()
{
    static std::unordered_map<std::string, detail::_component_def>
        s_registeredComponents;
    return s_registeredComponents;
}

} // namespace lemon
