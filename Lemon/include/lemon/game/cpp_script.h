#pragma once

#include "entity.h"

namespace lemon {
class cpp_script
{
  public:
    cpp_script(const entity& ent):
        ent(ent) { }
    virtual ~cpp_script() = default;

    virtual void on_create() { }
    virtual void on_update(float) { }
    virtual void on_late_update(float) { }
    virtual void on_destroy() { }

  protected:
    entity ent;
};
} // namespace lemon
