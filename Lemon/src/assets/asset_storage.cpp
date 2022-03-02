#include <lemon/assets/asset_storage.h>

#include <lemon/renderer/shader.h>
#include <lemon/renderer/texture.h>

namespace lemon {
ptr<asset_storage> asset_storage::storage = nullptr;
asset_storage::resource::resource():
    _stored{ nullptr }, _count{ 0 }
{ }
asset_storage::resource::resource(object* ptr):
    _stored(ptr), _count(1)
{ }
asset_storage::resource::resource(self_type&& other) noexcept:
    _stored(other._stored), _count(other._count.load())
{
    other._stored = nullptr;
}
asset_storage::resource::self_type&
asset_storage::resource::operator=(self_type&& other) noexcept
{
    if(this != &other)
    {
        std::swap(_stored, other._stored);
        _count.store(other._count.load(), std::memory_order_relaxed);
    }
    return *this;
}
asset_storage::resource::~resource()
{
    if(_stored)
        delete _stored;
}
void asset_storage::resource::increment() noexcept
{
    _count.fetch_add(1, std::memory_order_relaxed);
}
u32 asset_storage::resource::decrement() noexcept
{
    return _count.fetch_sub(1, std::memory_order_release);
}
bool asset_storage::resource::loaded() const noexcept
{
    return _stored != nullptr;
}
ptr<object> asset_storage::resource::get() noexcept
{
    return _stored;
}
ptr<object> asset_storage::resource::get() const noexcept
{
    return _stored;
}
void asset_storage::resource::set(ptr<object> ptr)
{
    _stored = ptr;
}
asset_storage::asset_storage():
    loader(create_owned<asset_loader>())
{
    LEMON_ASSERT(!storage);
    storage = this;

    auto _texture = string_id("mock_texture");
    auto _shader  = string_id("mock_shader");
    cachedAssets.insert(std::make_pair(
        _texture, resource(loader->load_resource<texture>(_texture))));
    cachedAssets.insert(std::make_pair(
        _shader, resource(loader->load_resource<shader>(_shader))));
    LOG_MESSAGE("Asset storage created");
}
asset_storage::~asset_storage()
{
    LOG_MESSAGE("Asset storage destroyed");
}
void asset_storage::release_asset(string_id name)
{
    if(cachedAssets.contains(name))
    {
        auto&& res = cachedAssets[name];
        if(res.decrement() == 1)
        {
            cachedAssets.erase(name);
        }
    }
}
string_id asset_storage::clone_asset(string_id name)
{
    if(cachedAssets.contains(name))
    {
        cachedAssets[name].increment();
    }
    return name;
}
} // namespace lemon
