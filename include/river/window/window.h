#include <GLFW/glfw3.h>
#include <string>

namespace river {
class window
{
  public:
    window(std::string const& name, int width, int height);
    ~window();

    bool should_close();
    void window_end_frame();

  private:
    GLFWwindow* handle;
};
}