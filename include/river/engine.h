#include "window/window.h"

namespace river {
class engine
{
  public:
    engine(int argc, char** argv);
    ~engine();
    void run();

  private:
    window* _window;

  protected:
    virtual void on_create()  = 0;
    virtual void on_destroy() = 0;
};
}