#pragma once

#include <river/game/cpp_script.h>
#include <river/input/input.h>
#include <river/input/key_codes.h>

using namespace river;

class editor_camera_controller : public cpp_script
{
  public:
    editor_camera_controller(const entity& ent);
    ~editor_camera_controller();

    void on_create();
    void on_update(float deltaTime);

  private:
    ptr<input> in;
};