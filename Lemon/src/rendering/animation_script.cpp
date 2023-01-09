#include <rendering/animation_script.h>

#include <filesystem>

namespace lemon {
namespace fs = std::filesystem;
animation_script::animation_script(hashstr nameid, const std::string& path, scripting_engine& _scriptingEngine):
    resource(nameid)
{
    this->name = fs::path(path).stem().string();
    _scriptingEngine.load_file(path);
    animations = _scriptingEngine.get_animation(name);
}

animation_script::~animation_script()
{
}

animation_script::animation_script(animation_script&& other) noexcept:
    resource(std::move(other)), name(std::move(other.name)), animations(std::move(other.animations))
{ }

animation_script& animation_script::operator=(animation_script&& other) noexcept
{
    if (this != &other)
    {
        resource::operator=(std::move(other));
        path = std::move(other.path);
        name = std::move(other.name);
        std::swap(animations, other.animations);
    }
    return *this;
}

i32 animation_script::execute(hashstr nameid, f32 value)
{
    auto a = animations.find(nameid);
    if (a == animations.end())
    {
        logger::error("Animation {} not found", nameid.str);
        return 0;
    }

    return a->second(value);
}
}
