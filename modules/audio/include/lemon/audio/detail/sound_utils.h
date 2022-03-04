#pragma once

#include <lemon/core/logger.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <string>

#define alCall(function, ...)          alCallImpl(function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(function, device, __VA_ARGS__)
namespace lemon {
static bool check_al_errors()
{
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        LOG_ERROR("OPENAL ERROR:");
        switch(error)
        {
        case AL_INVALID_NAME:
            LOG_ERROR("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
            break;
        case AL_INVALID_ENUM:
            LOG_ERROR("AL_INVALID_ENUM: an invalid enum was passed to an OpenAL function");
            break;
        case AL_INVALID_VALUE:
            LOG_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
            break;
        case AL_INVALID_OPERATION:
            LOG_ERROR("AL_INVALID_OPERATION: the requested operation is not valid");
            break;
        case AL_OUT_OF_MEMORY:
            LOG_ERROR("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
            break;
        default:
            LOG_ERROR("UNKNOWN AL ERROR");
        }
        return false;
    }
    return true;
}
template<class alFunction, class... Args>
static auto alCallImpl(alFunction function, Args&&... args)
    -> typename std::enable_if_t<!std::is_same_v<void, decltype(function(args...))>, decltype(function(args...))>
{
    auto ret = function(std::forward<Args>(args)...);
    check_al_errors();
    return ret;
}
template<class alFunction, class... Args>
static auto alCallImpl(alFunction function, Args&&... args)
    -> typename std::enable_if_t<std::is_same_v<void, decltype(function(args...))>, bool>
{
    function(std::forward<Args>(args)...);
    return check_al_errors();
}
static bool check_alc_errors(ALCdevice* device)
{
    ALCenum error = alcGetError(device);
    if(error != AL_NO_ERROR)
    {
        LOG_ERROR("OPENAL ERROR:");
        switch(error)
        {
        case ALC_INVALID_VALUE:
            LOG_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
            break;
        case ALC_INVALID_DEVICE:
            LOG_ERROR("AL_INVALID_ENUM: a bas device was passed to an OpenAL function");
            break;
        case ALC_INVALID_CONTEXT:
            LOG_ERROR("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
            break;
        case ALC_INVALID_ENUM:
            LOG_ERROR("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
            break;
        case ALC_OUT_OF_MEMORY:
            LOG_ERROR("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
            break;
        default:
            LOG_ERROR("UNKNOWN ALC ERROR");
        }
        return false;
    }
    return true;
}
template<class alFunction, class... Args>
static auto alcCallImpl(alFunction function, ALCdevice* device, Args&&... args)
    -> typename std::enable_if_t<std::is_same_v<void, decltype(function(args...))>, bool>
{
    function(std::forward<Args>(args)...);
    return check_alc_errors(device);
}
template<class alFunction, class ReturnType, class... Args>
static auto alcCallImpl(alFunction function, ReturnType& returnValue, ALCdevice* device, Args&&... args)
    -> typename std::enable_if_t<!std::is_same_v<void, decltype(function(args...))>, bool>
{
    returnValue = function(std::forward<Args>(args)...);
    return check_alc_errors(device);
}
} // namespace lemon
