#include <river/time/clock.h>

namespace river {
clock::clock():
    startTime(clock_type::now()), virtualFrameTime{}, deltaTime{},
    realDeltaTime{}, timeScale{ 1.0f }
{ }
clock::duration_type clock::elapsed_from_start(const time_point& tp) const noexcept
{
    return tp - startTime;
}
void clock::update() noexcept
{
    time_point _now          = clock_type::now();
    duration_type _realDelta = _now - realLastFrameTime;
    duration_type _delta     = duration_type(i64(_realDelta.count() * timeScale));

    realDeltaTime     = _realDelta.count() / 1000000000.0f;
    realLastFrameTime = _now;

    deltaTime = _delta.count() / 1000000000.0f;
    virtualFrameTime += _delta;
}
void clock::set_timescale(float timeScale) noexcept
{
    this->timeScale = timeScale;
}
} // namespace river
