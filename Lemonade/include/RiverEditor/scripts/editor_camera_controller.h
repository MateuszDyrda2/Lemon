#pragma once

#include <lemon/game/cpp_script.h>
#include <lemon/input/input.h>
#include <lemon/input/key_codes.h>

using namespace lemon;

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