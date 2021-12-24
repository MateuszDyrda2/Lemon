#pragma once
#include "object.h"

namespace river {
class entity;
class component : public object
{
  public:
    component(const std::string& name, size_type idx, bool enabled = true);
    component(string_id id, size_type idx, bool enabled = true);
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
