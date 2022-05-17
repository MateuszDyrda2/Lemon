#include <lemon/core/time/clock.h>

#include <lemon/core/logger.h>

#include <lemon/core/service_registry.h>

namespace lemon {
game_clock::game_clock(service_registry&):
    startTime(clock_type::now()), virtualFrameTime{},
    realLastFrameTime{ startTime }, deltaTime{},
    realDeltaTime{}, timeScale{ 1.0f }
{
    LOG_MESSAGE("Clock created");
}
game_clock::~game_clock()
{
    LOG_MESSAGE("Clock destroyed");
}
game_clock::duration_type game_clock::elapsed_from_start(const time_point& tp) const noexcept
{
    return tp - startTime;
}
void game_clock::update() noexcept
{
    time_point _now          = clock_type::now();
    duration_type _realDelta = _now - realLastFrameTime;
    duration_type _delta     = duration_type(i64(_realDelta.count() * timeScale));

    realDeltaTime     = _realDelta.count() / 1000000000.0f;
    realLastFrameTime = _now;

    deltaTime = _delta.count() / 1000000000.0f;
    virtualFrameTime += _delta;
}
void game_clock::set_timescale(f32 timeScale) noexcept
{
    this->timeScale = timeScale;
}
} // namespace lemon
