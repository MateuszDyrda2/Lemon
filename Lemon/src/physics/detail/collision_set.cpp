#include <physics/detail/collision_set.h>
#include <type_traits>

namespace lemon {
collision_set::collision_set()
{
    static_assert(is_correct::value);
}

bool collision_set::has(entity_t a, entity_t b)
{
    auto key = combine(entity_type(a), entity_type(b));
    return container.contains(key);
}

bool collision_set::push(entity_t a, entity_t b)
{
    auto key = combine(entity_type(a), entity_type(b));
    return container.insert(key).second;
}

void collision_set::clear()
{
    container.clear();
}

collision_set::key_t
collision_set::combine(entity_type a, entity_type b)
{
    auto&& [s, l] = std::minmax(a, b);
    return (key_t(l) << (sizeof(entity_type) * 8)) | s;
}

void collision_set::swap(collision_set& other) noexcept
{
    container.swap(other.container);
}

std::pair<entity_t, entity_t> collision_set::decouple(key_t key)
{
    auto s = entity_type(key);
    auto l = entity_type(key >> (sizeof(entity_type) * 8));
    return { entity_t(s), entity_t(l) };
}
}
