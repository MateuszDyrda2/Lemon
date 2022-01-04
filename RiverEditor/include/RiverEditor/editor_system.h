#pragma once

#include <river/game/system.h>

#include <river/game/scene.h>

using namespace river;

class editor_system : public system
{
  public:
    editor_system(ptr<river::scene> sc);
    ~editor_system();
    void update(entity_registry& registry) override;

  private:
    entity mainCamera;
};