#pragma once

#include "../scene/entity.h"

namespace lemon {
/** Base system interface */
class LEMON_PUBLIC system
{
  public:
    system()          = default;
    virtual ~system() = default;
    /** @brief Updates the components that belong to the system
     * @param registry entity registry
     */
    virtual void update(entity_registry& registry) = 0;
};
} // namespace lemon
