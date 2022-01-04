#pragma once

#include <lemon/game/system.h>

#include <lemon/game/scene.h>

using namespace lemon;

class editor_system : public system
{
  public:
    editor_system(ptr<lemon::scene> sc);
    ~editor_system();
    void update(entity_registry& registry) override;

  private:
    entity mainCamera;
};