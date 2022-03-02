#pragma once

#include "sound_utils.h"

#include <lemon/game/object.h>

#include <string>
#include <utility>
#include <vector>

namespace lemon {

class sound : object
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

  private:
    ALuint handle;
    info header;

    static std::pair<ALuint, info> load_sound(const std::string& path);
};
} // namespace lemon
