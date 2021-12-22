#pragma once
#include "object.h"

namespace river {
class entity;
class component : public object
{
    enum state
    {
        disabled = 0,
        active   = 1 << 0,
        enabled  = 1 << 1,
    };

  public:
    component(std::string const& name, bool enabled = true);
    component(string_id id, bool enabled = true);
    virtual ~component() = default;

    virtual void awake() { }
    virtual void start() { }
    virtual void update(float deltaTime) { }
    virtual void fixed_update(float fixedDelta) { }
    virtual void late_update(float deltaTime) { }
    virtual void on_enable() { }
    virtual void on_disable() { }
    virtual void on_destroy() { }

    bool is_enabled() const;
    bool is_active_and_enabled() const;

  protected:
    entity* _entity;

  private:
    state _state;

  private:
    void destroy_this() override;
    object* clone_this() override;
};
} // namespace river
