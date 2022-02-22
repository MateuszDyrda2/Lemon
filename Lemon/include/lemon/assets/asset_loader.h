#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/string_id.h>

#include <unordered_map>
#include <vector>

namespace lemon {
/** Class loading resources from files */
class asset_loader
{
  public:
    using self_type      = asset_loader;
    using container_type = std::unordered_map<string_id, std::string>;

  public:
    /** @brief Creates the asset loader, reading the specified
     * xml file containing paths of the resources used in the game
     */
    asset_loader();
    ~asset_loader();
    /** @brief Loads the resource from the file and returns the pointer to it
     * @param name id of the resource
     * @return pointer to the resource
     */
    template<class T>
    ptr<T> load_resource(string_id name);
    /** @brief Checks whether the resource with the specified name
     * is available for loading
     * @param name id of the resource
     * @return true if the resource exists
     */
    bool resource_exists(string_id name) const noexcept;

  private:
    container_type resourcePaths; ///< map of paths of the resources

  private:
    std::vector<byte> load_from_file(const std::string& path);
};
template<class T>
ptr<T> asset_loader::load_resource(string_id name)
{
    ptr<T> resource{};
    if(auto res = resourcePaths.find(name); res != resourcePaths.end())
    {
        resource = new T(name, load_from_file(res->second));
    }
    return resource;
}
} // namespace lemon
