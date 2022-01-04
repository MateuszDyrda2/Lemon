#pragma once

#include <lemon/core/string_id.h>

namespace lemon {
class object
{
  public:
    object(string_id id);
    object(const object&)     = default;
    object(object&&) noexcept = default;
    object& operator=(const object&) = default;
    object& operator=(object&&) noexcept = default;

    const char* get_name() const { return id.get_string(); }
    const string_id& get_id() const { return id; }
    bool operator==(const object& other);
    virtual ~object() = default;

  private:
    string_id id;
};
} // namespace lemon