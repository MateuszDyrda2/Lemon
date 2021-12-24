#pragma once

#include "renderer/renderer.h"
#include "window/window.h"
#include <memory>

namespace river {
class engine
{
  public:
    engine(int argc, char** argv);
    virtual ~engine();
    void run();

  private:
    std::unique_ptr<window> _window;
    std::unique_ptr<renderer> _renderer;
};
}