#pragma once

#include "camera.h"

namespace river {
class ortho_camera : public camera
{
  public:
    ortho_camera(const std::string& name, scene* this_scene);
    ortho_camera(string_id id, scene* this_scene);

  private:
    void recalculate_projection() override;
};
} // namespace river