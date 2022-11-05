#include <scripting/script_entity.h>

#include "rendering/components/rendering_components.h"

namespace lemon {
void script_animator::start_animation(const char* animation) const
{
    _scene->get_entity(entity_t(handle)).emplace<start_animation_m>(hash_string_d(animation));
}

void script_animator::stop_animations() const
{
    _scene->get_entity(entity_t(handle)).emplace<stop_animation_m>();
}

script_message_proxy& script_message_proxy::arg(i32 arg)
{
    builder.add_arg(arg);
    return *this;
}

script_message_proxy& script_message_proxy::arg(f32 arg)
{
    builder.add_arg(arg);
    return *this;
}

script_message_proxy& script_message_proxy::arg(bool arg)
{
    builder.add_arg(arg);
    return *this;
}

script_message_proxy& script_message_proxy::arg(vec2 arg)
{
    builder.add_arg(arg);
    return *this;
}

script_message_proxy& script_message_proxy::arg(const char* arg)
{
    builder.add_arg(hash_string_d(arg));
    return *this;
}

void script_message_proxy::push()
{
    builder.push();
}

script_message_proxy script_entity::create_message(const char* name)
{
    return script_message_proxy{ .builder = _messageBus.create_message(handle, name),
                                 .handle  = handle };
}
}
