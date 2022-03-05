#pragma once

#include <lemon/assets/resource.h>
#include <lemon/core/basic_types.h>

#include <functional>

namespace lemon {
class py_script : public resource
{
  public:
    py_script(string_id name, const std::string& path);
    virtual ~py_script();

  private:
    std::string sourceCode;

    std::function<void(f32)> onUpdate;
};
} // namespace lemon
