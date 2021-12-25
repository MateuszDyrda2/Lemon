#pragma once

#include "entity.h"
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

  private:
    std::vector<std::unique_ptr<entity>> entities;
    std::vector<size_type> toDestroy;
    std::unique_ptr<camera> _camera;

  private:
    friend class entity;
    void destroy_entity(entity* ent);
};
} // namespace river
