#pragma once

#include "window/window.h"

namespace river {
class engine
{
  public:
    engine(int argc, char** argv);
    virtual ~engine();
    void run();

  private:
    window* _window;
};
}