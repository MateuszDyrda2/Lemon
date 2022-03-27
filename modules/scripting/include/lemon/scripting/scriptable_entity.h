#pragma once

#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/entity.h>

namespace lemon {
struct LEMON_PUBLIC scriptable_entity
{
    scriptable_entity(entity ent):
        ent(ent) { }
    virtual ~scriptable_entity() = default;

    void set_transform(const transform& t);
    const transform& get_transform() const;

    const tag& get_tag() const;

    bool get_enabled() const;
    void set_enabled(bool value);

    entity ent;
};
} // namespace lemon
