#include <lemon/engine/engine.h>

using namespace lemon;
class sandbox : public engine
{
  public:
    sandbox(int argc, char** argv):
        engine(argc, argv, "/home/matzix/shared/Lemon_alt/sandbox/assets.json") { }
    ~sandbox() { }
};
int main(int argc, char** argv)
{
    auto eng = new sandbox(argc, argv);
    return eng->run();
}