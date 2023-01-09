/** @file scripting_engine.h
 * @brief File with a scripting engine definition
 */
#pragma once

#include <core/defines.h>
#include <core/hash_string.h>
#include <core/lemon_types.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <events/events.h>

struct lua_State;
namespace lemon {
/** Scripting engine class */
class LEMON_API scripting_engine
{
  public:
    /** @brief Creates an engine
     * @param _input input manager reference
     */
    scripting_engine(class input& _input, event_queue& _eventQueue);
    ~scripting_engine();
    /** @brief Loads a script file
     * @param path path to the file
     */
    void load_file(const std::string& path);
    /** @brief Gets the animation definitions
     * @param name animation file name
     * @return animation map
     */
    std::unordered_map<hashstr, std::function<i32(f32)>> get_animation(const std::string& name);
    /** @brief Calls function in scope
     * @param scope scope
     * @param func function name to call
     */
    void call_function(const std::string& scope, const std::string& func);
    /** @return get lua state */
    lua_State* get_state() { return L; }

  private:
    lua_State* L;

  private:
    void register_math();
    void register_types();
};
}
