#include <river/game/component.h>

namespace river {
bool component::is_enabled() const
{
    return !!(_state & state::enabled);
}
bool component::is_active_and_enabled() const
{
    return !!(_state & (state::enabled | state::active));
}
void component::destroy_this()
{
}
object* component::clone_this()
{
}

} // namespace river
