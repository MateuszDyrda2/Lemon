#pragma once

#include <river/core/string_id.h>

namespace river {
class object
{
  public:
    object(string_id id);
    object(std::string const& name);
    std::string const& get_name() const { id.get_string(); }
    string_id const& get_id() const { return id; }
    bool operator==(object const& other);
    static void destroy(object* obj);
    static object* clone(object* obj);
    virtual ~object();

  protected:
    virtual void destroy_this()  = 0;
    virtual object* clone_this() = 0;

  private:
    string_id id;
};
} // namespace river