#include <RiverEditor/scripts/editor_camera_controller.h>

#include <river/service/services.h>

editor_camera_controller::editor_camera_controller(const entity& ent):
    cpp_script(ent), in(services::get<input>())
{
}
editor_camera_controller::~editor_camera_controller()
{
}
void editor_camera_controller::on_create()
{
}
void editor_camera_controller::on_update(float deltaTime)
{
    if(in->is_mouse_pressed(key::mouse::button_1))
    {
    }
}
