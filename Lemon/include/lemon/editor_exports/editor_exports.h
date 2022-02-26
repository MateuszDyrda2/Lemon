#pragma once

#include <lemon/engine.h>

namespace lemon {
class LEMON_PUBLIC editor_engine : public engine
{
  public:
    editor_engine(const std::string& workingDirectory, int argc, char** argv);
    virtual ~editor_engine();

    void initialize();
    bool update();
    void drop();
};
} // namespace lemon

extern "C"
{
    LEMON_PUBLIC lemon::editor_engine* CreateEngine();
    LEMON_PUBLIC bool Update(lemon::editor_engine* engine);
    LEMON_PUBLIC void Initialize(lemon::editor_engine* engine);
    LEMON_PUBLIC void Drop(lemon::editor_engine* engine);
    LEMON_PUBLIC void DeserializeScene(const char* name);
    LEMON_PUBLIC void SerializeScene();
}
