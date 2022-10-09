#include <core/math/math.h>
#include <world/systems/transform_system.h>

namespace lemon {
transform_system::transform_system(scene& _scene, event_queue& _eventQueue):
    _scene(_scene)
{
    update = _eventQueue["Update"_hs] += [this](event_args* e) {
        this->onUpdate(e);
    };
}

transform_system::~transform_system() { }

void transform_system::onUpdate([[maybe_unused]] event_args* e)
{
    _scene.sort<dirty_t>([this](const auto lhs, const auto rhs) {
        const auto& clhs = _scene.get<transform>(lhs);
        const auto& crhs = _scene.get<transform>(rhs);
        return clhs.order < crhs.order;
    });

    _scene.view<dirty_t>().each(
        [this](const auto ent) {
            auto&& [_transform, _model] =
                _scene.get<transform, model>(ent);

            _model.matrix = _transform.parent == entt::null
                                ? mat4(1.0f)
                                : _scene.get<model>(_transform.parent).matrix;
            _model.matrix = translate(_model.matrix, vec3(_transform.position, 0.0f));
            _model.matrix = rotate(_model.matrix, _transform.rotation, vec3(0.0f, 0.0f, 1.0f));
            _model.matrix = scale(_model.matrix, vec3(_transform.scale, 1.0f));
        });
    _scene.clear<dirty_t>();
}

void transform_system::move_to(entity& _entity, vec2 newPosition)
{
    auto&& tr   = _entity.get<transform>();
    tr.position = newPosition;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::move_by(entity& _entity, vec2 distance)
{
    auto&& tr = _entity.get<transform>();
    tr.position += distance;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::rotate_to(entity& _entity, f32 newRotation)
{
    auto&& tr   = _entity.get<transform>();
    tr.rotation = newRotation;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::rotate_by(entity& _entity, f32 degrees)
{
    auto&& tr = _entity.get<transform>();
    tr.rotation += degrees;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::scale_to(entity& _entity, vec2 newScale)
{
    auto&& tr = _entity.get<transform>();
    tr.scale  = newScale;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::scale_by(entity& _entity, vec2 scale)
{
    auto&& tr = _entity.get<transform>();
    tr.scale *= scale;
    _entity.emplace_or_replace<dirty_t>();
}
}
