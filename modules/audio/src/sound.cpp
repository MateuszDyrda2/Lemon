#include <lemon/audio/sound.h>

#include <AL/alext.h>
#include <wave/file.h>

namespace lemon {
using namespace std;
sound::sound(string_id name, const std::string& path):
    resource(name)
{
    wave::File readFile;
    auto err = readFile.Open(path, wave::kIn);
    if(err) { LOG_ERROR("file reading error"); }
    vector<f32> buffer;
    err = readFile.Read(&buffer);
    if(err) { LOG_ERROR("file reading error"); }
    ALenum format;
    if(readFile.channel_number() == 1)
    {
        format = AL_FORMAT_MONO_FLOAT32;
    }
    else if(readFile.channel_number() == 2)
    {
        format = AL_FORMAT_STEREO_FLOAT32;
    }
    else
    {
        LOG_ERROR("Unusported channel count");
    }

    ALsizei numBytes = (ALsizei)(readFile.frame_number()
                                 * readFile.channel_number())
                       * (ALsizei)sizeof(f32);

    handle              = 0;
    header.sampleRate   = readFile.sample_rate();
    header.bitDepth     = readFile.bits_per_sample();
    header.channelCount = readFile.channel_number();
    header.lenght       = numBytes / (header.sampleRate * header.channelCount * header.bitDepth / 8);

    alCall(alGenBuffers, 1, &handle);
    alCall(alBufferData, handle, format, buffer.data(), numBytes, readFile.sample_rate());

    if(!handle)
    {
        LOG_ERROR("Audio file failed: %s", name.get_string());
    }
}
sound::sound(string_id name, const std::vector<byte>& buffer):
    resource(name)
{
}
sound::~sound()
{
    if(handle)
    {
        alCall(alDeleteBuffers, 1, &handle);
    }
}
} // namespace lemon
