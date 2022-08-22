 #pragma once

#include <serialization/scene_serializer.h>
#include <core/defines.h>
#include <rendering/components/rendering_components.h>
#include <world/components/entity_components.h>
#include <world/components/transform_components.h>


namespace lemon {
static component_list_t<sprite_renderer,tag,transform,model,camera,enabled_t,main_camera_t> componentList;
}