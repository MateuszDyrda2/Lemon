#include <lemon/assets/asset_storage.h>

namespace lemon {
resource::resource():
    _stored{ nullptr }, _count{ 0 }
{ }
resource::resource(object* ptr):
    _stored(ptr), _count(1)
{ }
resource::resource(self_type&& other) noexcept:
    _stored(other._stored), _count(other._count.load())
{
    other._stored = nullptr;
}
resource::self_type& resource::operator=(self_type&& other) noexcept
{
    if(this != &other)
    {
        std::swap(_stored, other._stored);
        _count.store(other._count.load(), std::memory_order_relaxed);
    }
    return *this;
}
resource::~resource()
{
    if(_stored)
        delete _stored;
}
void resource::increment() noexcept
{
    _count.fetch_add(1, std::memory_order_relaxed);
}
u32 resource::decrement() noexcept
{
    return _count.fetch_sub(1, std::memory_order_release);
}
bool resource::loaded() const noexcept
{
    return _loaded;
}
ptr<object> resource::get() noexcept
{
    return _stored;
}
ptr<object> resource::get() const noexcept
{
    return _stored;
}
asset_storage::asset_storage(const std::string& dataPath):
    loader(create_owned<asset_loader>(dataPath))
{
    asset_impl::storage = this;
}
asset_storage::~asset_storage()
{
}
ptr<object> asset_storage::get_asset(string_id name) const
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        return res->second.get();
    }
    return nullptr;
}
void asset_storage::release_asset(string_id name)
{
    LEMON_ASSERT(cachedAssets.contains(name));
    auto&& res = cachedAssets[name];
    if(res.decrement() == 1)
    {
        cachedAssets.erase(name);
    }
}
void asset_storage::clone_asset(string_id name)
{
    LEMON_ASSERT(cachedAssets.contains(name));
    cachedAssets[name].increment();
}
namespace asset_impl {
ptr<asset_storage> storage = nullptr;
} // namespace asset_impl
} // namespace lemon
