#include <assets/asset.h>

#include <core/assert.h>
#include <core/logger.h>

namespace lemon {
asset_storage::asset_storage(const std::string& path):
    loader(std::make_unique<asset_loader>(path))
{
    logger::info("Asset storage created");
}
asset_storage::~asset_storage()
{
    logger::info("Asset storage destroyed");
}
void asset_storage::release_asset(hash_str nameid)
{
    if(auto res = cachedAssets.find(nameid);
       res != cachedAssets.end())
    {
        if(res->second->decrement() == 1)
        {
            toDelete.push_back(std::move(res->second));
            cachedAssets.erase(res);
        }
    }
}
hash_str asset_storage::clone_asset(hash_str nameid)
{
    if(auto res = cachedAssets.find(nameid);
       res != cachedAssets.end())
    {
        res->second->increment();
    }
    return nameid;
}
void asset_storage::update()
{
    toDelete.clear();
}
} // namespace lemon
