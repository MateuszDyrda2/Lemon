#include <rendering/animation_script.h>

namespace lemon {

animation_script::animation_script(hash_str name, const std::string& path, scripting_engine& _scriptingEngine):
    resource(name)
{
    _scriptingEngine.load_file(path);
}

animation_script::~animation_script()
{
}

animation_script::animation_script(animation_script&& other) noexcept
{
}

animation_script& animation_script::operator=(const animation_script&& other) noexcept
{
}
}
