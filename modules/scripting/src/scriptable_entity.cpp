#include <lemon/scripting/scriptable_entity.h>

#include <lemon/core/logger.h>

namespace lemon {
void scriptable_entity::set_transform(const transform& t)
{
    LOG_WARN("Setter called");
    ent.change_component<transform>(t);
}
const transform& scriptable_entity::get_transform() const
{
    LOG_WARN("Getter called");
    return ent.get_component<transform>();
}
const tag& scriptable_entity::get_tag() const
{
    return ent.get_component<tag>();
}
bool scriptable_entity::get_enabled() const
{
    return ent.has_component<enabled_t>();
}
void scriptable_entity::set_enabled(bool value)
{
    if(value)
        ent.emplace_or_replace<enabled_t>();
    else
        ent.remove_component<enabled_t>();
}
} // namespace lemon