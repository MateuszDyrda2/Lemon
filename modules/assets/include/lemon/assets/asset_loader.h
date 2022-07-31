#pragma once

#include <lemon/core/hash_string.h>
#include <lemon/core/lemon_types.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace lemon {
/** Class loading resources from files */
class asset_loader
{
  public:
    using self_type      = asset_loader;
    using container_type = std::unordered_map<hash_str, std::string>;

  public:
    /** @brief Creates the asset loader, reading the specified
     * json file containing paths of the resources used in the game
     * @param path path to the resource file
     */
    asset_loader(const std::string& path);
    ~asset_loader();
    /** @brief Loads the resource from the file and returns the pointer to it
     * @param nameid id of the resource
     * @return pointer to the resource
     */
    template<class T>
    std::unique_ptr<T> load_resource(hash_str nameid);
    /** @brief Checks whether the resource with the specified name
     * is available for loading
     * @param nameid id of the resource
     * @return true if the resource exists
     */
    bool resource_exists(hash_str nameid) const noexcept;

  private:
    container_type resourcePaths; ///< map of paths of the resources
    std::string path;

  private:
    std::vector<char> load_from_file(const std::string& path);
};
template<class T>
inline std::unique_ptr<T> asset_loader::load_resource(hash_str nameid)
{
    return std::make_unique<T>(nameid, resourcePaths[nameid]);
}
} // namespace lemon
