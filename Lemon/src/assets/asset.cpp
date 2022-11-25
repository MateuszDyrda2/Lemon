#include <assets/asset.h>

namespace lemon {
asset_storage::asset_storage(const std::string& path, scripting_engine& _scriptingEngine):
    loader(std::make_unique<asset_loader>(path, _scriptingEngine))
{
    logger::info("Asset storage created");
}
asset_storage::~asset_storage()
{
    logger::info("Asset storage destroyed");
}
void asset_storage::release_asset(hashstr nameid)
{
    if (!nameid) return;

    auto res = cachedAssets.find(nameid);
    if (res == cachedAssets.end()) return;

    if (res->second->decrement() > 1) return;

    toDelete.push_back(std::move(res->second));
    cachedAssets.erase(res);
}
hashstr asset_storage::clone_asset(hashstr nameid)
{
    if (!nameid) return nameid;

    auto res = cachedAssets.find(nameid);
    if (res == cachedAssets.end()) return nameid;

    res->second->increment();
    return nameid;
}
void asset_storage::update()
{
    toDelete.clear();
}
} // namespace lemon
