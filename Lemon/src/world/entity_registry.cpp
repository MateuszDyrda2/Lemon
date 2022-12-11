#include <world/entity_registry.h>

namespace lemon {
void entity_registry::push(hashstr name, entity_t handle)
{
    container[name] = handle;
}

void entity_registry::pop(hashstr name)
{
    lemon_assert(container.contains(name));
    container.erase(name);
}

entity_t entity_registry::get(hashstr name) const
{
    if (!container.contains(name)) logger::warn("Did not find: {}", name.str);
    return container.at(name);
}
}
