#pragma once
#include <rendering\components\rendering_components.h>
#include <serialization/scene_serializer.h>
#include <world\components\entity_components.h>
#include <world\components\transform_components.h>

namespace lemon {
static component_list_t<sprite_renderer, enabled_t, tag, transform, model, main_camera_t, camera> componentList;
}
