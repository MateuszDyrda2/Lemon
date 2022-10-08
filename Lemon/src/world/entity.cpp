#include <world/entity.h>

#include <world/components/transform_components.h>

namespace lemon {
void entity::set_dirty()
{
    lemon_assert(reg);
    reg->emplace_or_replace<dirty_t>(handle);
}
}
