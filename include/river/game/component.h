#pragma once
#include "object.h"

namespace river {
#define COMPONENT_BODY(COMPONENT_NAME)                                 \
  public:                                                              \
    COMPONENT_NAME(entity* _ent__, size_type _idx__, bool _enabled__); \
                                                                       \
  private:

#define CONSTRUCTOR_BODY(COMPONENT_NAME)                                               \
    COMPONENT_NAME::COMPONENT_NAME(entity* _ent__, size_type _idx__, bool _enabled__): \
        component(_ent__, _idx__, _enabled)

class entity;
class component : public object
{
  public:
    component(entity* _ent, size_type idx, bool enabled);
    virtual ~component() = default;

    virtual void awake() { }
    virtual void start() { }
    virtual void on_enable() { }
    virtual void on_disable() { }
    virtual void on_destroy() { }

    bool is_enabled() const;
    bool is_active_and_enabled() const;

    void enable();
    void disable();

  protected:
    entity* _entity;

  private:
    size_type _entityIndex;
    u8 active : 1;
    u8 enabled : 1;
    friend entity;

  private:
    void destroy_this();
    object* clone_this();
};
} // namespace river
