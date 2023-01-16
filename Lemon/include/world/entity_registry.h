/** @file entity_registry.h
 * @brief File with registry of entity handles
 *
 */
#pragma once

#include "core/defines.h"
#include "core/hash_string.h"
#include "world/system.h"
#include <unordered_map>

namespace lemon {
/** Entity names registry */
class LEMON_API entity_registry
{
  public:
    /** @brief Add new entity
     * @param name name of the entity
     * @param handle handle of the entity
     *
     */
    void push(hashstr name, entity_t handle);
    /** @brief Remove an entity
     * @param name name to remove
     */
    void pop(hashstr name);
    /** @brief Get entity handle from name
     * @param name name of the entity
     * @return handle of the entity
     */
    entity_t get(hashstr name) const;

  private:
    std::unordered_map<hashstr, entity_t> container;
};
}
