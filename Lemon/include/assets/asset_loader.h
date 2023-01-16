/** @file asset_loader.h
 * @brief File containing definitions for asset loading class
 */
#pragma once

#include "rendering/animation_script.h"
#include "scripting/scripting_engine.h"
#include <core/defines.h>
#include <core/hash_string.h>
#include <core/lemon_types.h>
#include <scripting/script.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace lemon {
/** Class loading resources from files */
class LEMON_API asset_loader
{
  public:
    using self_type      = asset_loader;
    using container_type = std::unordered_map<hashstr, std::string>;

  public:
    /** @brief Creates the asset loader, reading the specified
     * json file containing paths of the resources used in the game
     * @param path path to the resource file
     */
    asset_loader(const std::string& path, scripting_engine& _scriptingEngine);
    ~asset_loader();
    /** @brief Loads the resource from the file and returns the pointer to it
     * @param nameid id of the resource
     * @return pointer to the resource
     */
    template<class T>
    std::unique_ptr<T> load_resource(hashstr nameid);
    /** @brief Checks whether the resource with the specified name
     * is available for loading
     * @param nameid id of the resource
     * @return true if the resource exists
     */
    bool resource_exists(hashstr nameid) const noexcept;

  private:
    container_type resourcePaths; ///< map of paths of the resources
    std::string path;
    scripting_engine& _scriptingEngine;

  private:
    std::vector<char> load_from_file(const std::string& path);
};

template<class T>
inline std::unique_ptr<T> asset_loader::load_resource(hashstr nameid)
{
    return std::make_unique<T>(nameid, resourcePaths[nameid]);
}

template<>
inline std::unique_ptr<animation_script>
asset_loader::load_resource<animation_script>(hashstr nameid)
{
    return std::make_unique<animation_script>(nameid, resourcePaths[nameid], _scriptingEngine);
}

template<>
inline std::unique_ptr<script>
asset_loader::load_resource<script>(hashstr nameid)
{
    return std::make_unique<script>(nameid, resourcePaths[nameid], _scriptingEngine);
}
} // namespace lemon
