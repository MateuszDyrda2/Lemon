#pragma once

#include "entity.h"

namespace river {
class cpp_script
{
  public:
    cpp_script()          = default;
    virtual ~cpp_script() = default;

    virtual void on_create() { }
    virtual void on_update(float) { }
    virtual void on_late_update(float) { }
    virtual void on_destroy() { }

  protected:
    entity ent;
};
} // namespace river
