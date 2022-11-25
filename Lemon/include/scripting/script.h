#pragma once

#include "scripting/script_entity.h"
#include "scripting_engine.h"

#include <assets/resource.h>
#include <core/defines.h>
#include <core/hash_string.h>

namespace lemon {
struct message_payload;
class LEMON_API script : public resource
{
  public:
    script(hashstr nameid, const std::string& path, scripting_engine& _scriptingEngine);
    virtual ~script();

    script(const script&)            = delete;
    script& operator=(const script&) = delete;
    script(script&& other) noexcept;
    script& operator=(script&& other) noexcept;

    const std::string& get_path() const { return path; }

    static hashstr get_mocked() { return "mock_script"_hs; }

    void execute(script_entity ent, const std::string& func, message_payload* payload);

  private:
    std::string path;
    std::string name;
    scripting_engine& engine;
};
}
