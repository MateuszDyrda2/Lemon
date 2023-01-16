/** @file script.h
 * @brief File with a script lua definition
 */
#pragma once

#include "scripting/script_entity.h"
#include "scripting_engine.h"

#include <assets/resource.h>
#include <core/defines.h>
#include <core/hash_string.h>

namespace lemon {
struct message_payload;
/** Class representation of a lua script */
class LEMON_API script : public resource
{
  public:
    /** @brief Creates a script
     * @param nameid id of the script
     * @param path path of the script resource
     * @param _scriptingEngine engine reference
     */
    script(hashstr nameid, const std::string& path, scripting_engine& _scriptingEngine);
    virtual ~script();

    script(const script&)            = delete;
    script& operator=(const script&) = delete;
    script(script&& other) noexcept;
    script& operator=(script&& other) noexcept;
    /** @return script path */
    const std::string& get_path() const { return path; }
    /** @return id of the mocked script */
    static hashstr get_mocked() { return "mock_script"_hs; }
    /** @brief Run function in the script
     * @param ent entity to run function for
     * @param func function name to run
     * @param payload arguments
     */
    void execute(script_entity ent, const std::string& func, message_payload* payload);
    /** @brief Run update function
     * @param ent entity to run update for
     * @param delta delta time
     */
    void update(script_entity ent, f32 delta);
    /** @brief Run early update function
     * @param ent entity to run update for
     * @param delta delta time
     */
    void early(script_entity ent, f32 delta);
    /** @brief Run physics update function
     * @param ent entity to run update for
     * @param delta delta time
     */
    void physics(script_entity ent, f32 delta);
    /** @brief Run init function
     * @param ent entity to run start for
     */
    void start(script_entity ent);

  private:
    std::string path;
    std::string name;
    scripting_engine& engine;
    std::function<void(script_entity, f32)> update_f;
    std::function<void(script_entity, f32)> early_f;
    std::function<void(script_entity, f32)> physics_f;
};
}
