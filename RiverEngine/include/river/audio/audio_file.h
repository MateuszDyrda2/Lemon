#pragma once

#include "sound_utils.h"
#include <river/game/object.h>

#include <vector>

namespace river {
class audio_file
{
  public:
    audio_file(string_id name, const std::string& path);
    ~audio_file();

    u16 get_number_of_channels() const;
    u32 get_sample_rate() const;
    u16 get_bits_per_sample() const;
    u64 get_frame_number() const;

  private:
    ALuint buffer;
    ALenum format;
};
} // namespace river
