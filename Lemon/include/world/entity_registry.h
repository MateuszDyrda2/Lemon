#pragma once

#include "core/defines.h"
#include "core/hash_string.h"
#include "world/system.h"
#include <unordered_map>

namespace lemon {
class LEMON_API entity_registry
{
  public:
    void push(hashstr name, entity_t handle);
    void pop(hashstr name);
    entity_t get(hashstr name) const;

  private:
    std::unordered_map<hashstr, entity_t> container;
};
}
