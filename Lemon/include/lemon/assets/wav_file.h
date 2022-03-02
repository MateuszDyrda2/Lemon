#pragma once

#include <lemon/core/basic_types.h>

#include <vector>

namespace lemon {
namespace wav {
alignas(1) struct header
{
    u16 audioFormat;
    u16 numOfChannels;
    u32 sampleRate;
    u32 byteRate;
    u16 blockAlign;
    u16 bitsPerSample;
};
}
class wav_file
{
  public:
    wav_file(const std::vector<bytes>& buffer);
    inline const wav::header& get_header() const { return data; }
    inline const std::vector<byte>& get_data() const { return header; }
    ~wav_file();

  private:
    std::vector<byte> data;
    wav::header header;
};
} // namespace lemon
