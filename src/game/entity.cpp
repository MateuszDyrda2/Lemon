#include <river/game/entity.h>

#include <algorithm>
#include <river/game/scene.h>

namespace river {
entity::entity(std::string const& name, scene* this_scene):
    object(name), _transform(new transform(get_id())), _scene(this_scene)
{
}
entity::entity(string_id id, scene* this_scene):
    object(id), _transform(new transform(get_id())), _scene(this_scene)
{
}
entity::~entity()
{
    disable();
    std::for_each(components.begin(), components.end(),
                  [](std::unique_ptr<component>& cmp) {
                      cmp->disable();
                      cmp->on_destroy();
                  });
}
void entity::awake_components()
{
    active = 1;
    std::for_each(components.begin(), components.end(),
                  [](std::unique_ptr<component>& cmp) {
                      cmp->awake();
                      cmp->active = 1;
                      if(cmp->is_enabled()) cmp->on_enable();
                  });
}
void entity::start()
{
    std::for_each(components.begin(), components.end(),
                  [](std::unique_ptr<component>& cmp) {
                      if(cmp->is_enabled()) cmp->start();
                  });
}
void entity::destroy_pending()
{
    std::for_each(toDestroy.begin(), toDestroy.end(),
                  [this](size_type& index) {
                      auto it = components.begin() + componentIndexes[index];
                      (*it)->disable();
                      (*it)->on_destroy();
                      components.erase(components.begin() + componentIndexes[index]);
                      std::for_each(componentIndexes.begin() + index, componentIndexes.end(),
                                    [](size_type& index) {
                                        --index;
                                    });
                  });
    toDestroy.clear();
}
void entity::on_enable()
{
    std::for_each(components.begin(), components.end(),
                  [](std::unique_ptr<component>& cmp) {
                      if(cmp->is_enabled()) cmp->on_enable();
                  });
}
void entity::on_disable()
{
    std::for_each(components.begin(), components.end(),
                  [](std::unique_ptr<component>& cmp) {
                      if(cmp->is_enabled()) cmp->on_disable();
                  });
}
void entity::set_active(bool value)
{
    active = value;
}
void entity::enable()
{
    if(!enabled)
    {
        enabled = 1;
        on_enable();
    }
}
void entity::disable()
{
    if(enabled)
    {
        enabled = 0;
        on_disable();
    }
}
void entity::destroy_this()
{
    _scene->destroy_entity(this);
}
object* entity::clone_this()
{
    return nullptr;
}
void entity::destroy_component(size_type index)
{
    toDestroy.push_back(index);
}
size_type entity::get_index()
{
    static size_type idx = 0;
    return idx++;
}
} // namespace river
