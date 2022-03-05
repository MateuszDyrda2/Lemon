#include <lemon/assets/resource.h>

namespace lemon {
resource::resource(string_id name):
    _count{ 1 }, name(name)
{ }
resource::resource(self_type&& other) noexcept:
    _count(other._count.load()), name(other.name)
{
}
resource::self_type&
resource::operator=(self_type&& other) noexcept
{
    if(this != &other)
    {
        _count.store(other._count.load(), std::memory_order_relaxed);
        name = other.name;
    }
    return *this;
}
void resource::increment() noexcept
{
    _count.fetch_add(1, std::memory_order_relaxed);
}
u32 resource::decrement() noexcept
{
    return _count.fetch_sub(1, std::memory_order_release);
}
} // namespace lemon
