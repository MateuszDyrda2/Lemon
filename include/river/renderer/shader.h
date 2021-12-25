#pragma once

#include <glad/glad.h>
#include <river/game/object.h>

namespace river {
class shader : public object
{
  public:
    shader(const std::string& vertexPath, const std::string& fragmentPath);
    void use();

  private:
    unsigned int ID;
};
} // namespace river
