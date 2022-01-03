#include <river/audio/audio_file.h>

#include <river/core/logger.h>

#include <AL/alext.h>

namespace river {
audio_file::audio_file(string_id name, const std::string& path)
{
}
audio_file::~audio_file()
{
}
u16 audio_file::get_number_of_channels() const
{
}
u32 audio_file::get_sample_rate() const
{
}
u16 audio_file::get_bits_per_sample() const
{
}
u64 audio_file::get_frame_number() const
{
}
} // namespace river
