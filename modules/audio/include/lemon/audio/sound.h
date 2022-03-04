#pragma once

#include "detail/sound_utils.h"

#include <lemon/assets/resource.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

#include <string>
#include <utility>
#include <vector>

namespace lemon {

class LEMON_PUBLIC sound : public resource
{
  public:
    struct info
    {
        u32 sampleRate;
        u32 bitDepth;
        f64 lenght;
        u32 channelCount;
    };

  public:
    sound(string_id name, const std::string& path);
    sound(string_id name, const std::vector<byte>& buffer);
    ~sound();

    [[nodiscard]] const info& get_info() const { return header; }
    [[nodiscard]] ALuint get_handle() const { return handle; }

    static string_id get_mocked() { return string_id("mock_sound"); }

  private:
    ALuint handle;
    info header;
};
} // namespace lemon
