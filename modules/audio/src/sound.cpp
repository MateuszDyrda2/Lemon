#include <lemon/audio/sound.h>

#include <sndfile.h>

namespace lemon {
using namespace std;
sound::sound(string_id name, const std::string& path):
    resource(name)
{
    ALenum format;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    i16* membuf;
    sf_count_t numFrames;
    ALsizei numBytes;

    // open the audio file
    sndfile = sf_open(path.c_str(), SFM_READ, &sfinfo);
    if(!sndfile)
    {
        LOG_ERROR("Can't open file from: %s : %s", path.c_str(), sf_strerror(sndfile));
        return;
    }
    if(sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
    {
        LOG_ERROR("Bad sample count from: %s : %d", path.c_str(), path.c_str(), sfinfo.frames);
        return;
    }
    format = AL_NONE;
    if(sfinfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if(sfinfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else
    {
        LOG_ERROR("Unsupported channel count: %d", sfinfo.channels);
        return;
    }
    membuf    = new i16[(size_type(sfinfo.frames)) * sfinfo.channels];
    numFrames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if(numFrames < 1)
    {
        delete[] membuf;
        sf_close(sndfile);
        LOG_ERROR("Failed to read samples from %s", path.c_str());
        return;
    }
    numBytes = (ALsizei)(numFrames * sfinfo.channels) * (ALsizei)sizeof(i16);

    handle              = 0;
    header.sampleRate   = sfinfo.samplerate;
    header.bitDepth     = 16;
    header.channelCount = sfinfo.channels;
    header.lenght       = numBytes / (header.sampleRate * header.channelCount * header.bitDepth / 8);

    alCall(alGenBuffers, 1, &handle);
    alCall(alBufferData, handle, format, membuf, numBytes, sfinfo.samplerate);

    delete[] membuf;
    sf_close(sndfile);

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
    if(handle) alCall(alDeleteBuffers, 1, &handle);
}
} // namespace lemon
