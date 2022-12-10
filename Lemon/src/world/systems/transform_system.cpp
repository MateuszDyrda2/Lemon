#include <world/systems/transform_system.h>

#include <core/math/math.h>
#include <core/math/vec3.h>

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
    _scene.view<dirty_t>().each(
        [this](const auto ent) {
            auto&& [_transform, _model] =
                _scene.get<transform, model>(ent);

            /*            _model.matrix = mat4(1.0f);*/
            /*_model.matrix = translate(_model.matrix, vec3(_transform.position, (f32(_transform.layer) * 0.1f)));*/
            /*_model.matrix = rotate(_model.matrix, _transform.rotation, vec3(0.0f, 0.0f, 1.0f));*/
            /*_model.matrix = scale(_model.matrix, vec3(_transform.scale, 1.0f));*/
            _model.matrix = scale(vec3(_transform.scale, 1.0f)) * rotate(_transform.rotation, vec3(0.0f, 0.0f, 1.0f)) * translate(vec3(_transform.position, _transform.layer));
        });
    _scene.clear<dirty_t>();
}

transform& transform_system::get_transform(entity _entity)
{
    lemon_assert(_entity.has<transform>());
    return _entity.get<transform>();
}

void transform_system::move_to(entity _entity, vec2 newPosition)
{
    lemon_assert(_entity.has<transform>());
    auto&& tr   = _entity.get<transform>();
    tr.position = newPosition;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::move_by(entity& _entity, vec2 distance)
{
    lemon_assert(_entity.has<transform>());
    auto&& tr = _entity.get<transform>();
    tr.position += distance;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::rotate_to(entity& _entity, f32 newRotation)
{
    lemon_assert(_entity.has<transform>());
    auto&& tr   = _entity.get<transform>();
    tr.rotation = newRotation;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::rotate_by(entity& _entity, f32 degrees)
{
    lemon_assert(_entity.has<transform>());
    auto&& tr = _entity.get<transform>();
    tr.rotation += degrees;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::scale_to(entity& _entity, vec2 newScale)
{
    lemon_assert(_entity.has<transform>());
    auto&& tr = _entity.get<transform>();
    tr.scale  = newScale;
    _entity.emplace_or_replace<dirty_t>();
}

void transform_system::scale_by(entity& _entity, vec2 scale)
{
    lemon_assert(_entity.has<transform>());
    auto&& tr = _entity.get<transform>();
    tr.scale *= scale;
    _entity.emplace_or_replace<dirty_t>();
}

vec2 transform_system::lerp(vec2 start, vec2 end, f32 time)
{
    return start + (end - start) * time;
}
}
