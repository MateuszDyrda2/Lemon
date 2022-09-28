#pragma once

#include <core/math/math.h>
#include <world/systems/transform_system.h>

namespace lemon {
transform_system::transform_system(registry& _registry):
    _registry(_registry)
{
}

transform_system::~transform_system() { }

void transform_system::update()
{
    _registry.sort<dirty_t>([this](const entity lhs, const entity rhs) {
        const auto& clhs = _registry.get<transform>(lhs);
        const auto& crhs = _registry.get<transform>(rhs);
        return clhs.order < crhs.order;
    });

    _registry.view<dirty_t>().each(
        [this](const entity ent) {
            auto&& [_transform, _model] =
                _registry.get<transform, model>(ent);

            _model.matrix = _transform.parent == entt::null
                                ? mat4(1.0f)
                                : _registry.get<model>(_transform.parent).matrix;
            _model.matrix = translate(_model.matrix, vec3(_transform.position, 0.0f));
            _model.matrix = rotate(_model.matrix, _transform.rotation, vec3(0.0f, 0.0f, 1.0f));
            _model.matrix = scale(_model.matrix, vec3(_transform.scale, 1.0f));
        });
    _registry.clear<dirty_t>();
}
}
