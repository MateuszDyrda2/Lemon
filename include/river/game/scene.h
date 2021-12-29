#pragma once

#include "entity.h"
#include <river/core/basic_types.h>
#include <river/renderer/camera.h>

namespace river {
class scene : public object
{
  public:
    scene(const std::string& name);
    ~scene();
    void initialize();
    void update();
    void begin();

    entity& add_entity(const std::string& name);
    ptr<camera> get_main_camera() const { return _camera.get(); }

  private:
    std::vector<owned<entity>> entities;
    std::vector<size_type> toDestroy;
    owned<camera> _camera;

  private:
    friend class entity;
    void destroy_entity(ptr<entity> ent);
};
} // namespace river
