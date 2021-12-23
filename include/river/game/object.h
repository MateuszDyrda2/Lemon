#pragma once

#include <river/core/string_id.h>

namespace river {
class object
{
  public:
    object(string_id id);
    object(const std::string& name);
    const std::string& get_name() const { return id.get_string(); }
    const string_id& get_id() const { return id; }
    bool operator==(const object& other);
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