#include <lemon/assets/asset_storage.h>

#include <lemon/core/assert.h>
#include <lemon/core/logger.h>

namespace lemon {
ptr<asset_storage> asset_storage::storage = nullptr;

asset_storage::asset_storage():
    loader(create_owned<asset_loader>())
{
    LEMON_ASSERT(!storage);
    storage = this;

    auto _texture = string_id("mock_texture");
    auto _shader  = string_id("mock_shader");
    /*
cachedAssets.insert(std::make_pair(
    _texture, resource(loader->load_resource<texture>(_texture))));
cachedAssets.insert(std::make_pair(
    _shader, resource(loader->load_resource<shader>(_shader))));
    */
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
        auto&& res = cachedAssets.at(name);
        if(res->decrement() == 1)
        {
            cachedAssets.erase(name);
        }
    }
}
string_id asset_storage::clone_asset(string_id name)
{
    if(cachedAssets.contains(name))
    {
        cachedAssets.at(name)->increment();
    }
    return name;
}
} // namespace lemon
