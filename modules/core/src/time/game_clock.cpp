#include <lemon/core/time/game_clock.h>

#include <lemon/core/logger.h>
#include <lemon/core/service_registry.h>

namespace lemon {
game_clock::game_clock(service_registry&):
    startTime(clock_type::now()), virtualFrameTime{},
    realLastFrameTime{ startTime }, physicsLastFrameTime{},
    deltaTime{},
    physicsDeltaTime{ 1'000'000'000.0 / 100.0 },
    realDeltaTime{}, timeScale{ 1.0f }, accumulator{ 0.0 },
    alpha{ 0.0 }
{
    LOG_MESSAGE("Clock created");
}
game_clock::~game_clock()
{
    LOG_MESSAGE("Clock destroyed");
}
void game_clock::calculate_delta_time() noexcept
{
    auto _now         = clock_type::now();
    realDeltaTime     = (_now - realLastFrameTime).count();
    realLastFrameTime = _now;

    deltaTime = realDeltaTime * timeScale;
    virtualFrameTime += deltaTime;
}
void game_clock::set_timescale(f32 timeScale) noexcept
{
    this->timeScale = timeScale;
}
f64 game_clock::get_real_time() const noexcept
{
    return nano2sec((realLastFrameTime - startTime).count());
}
f64 game_clock::get_time() const noexcept
{
    return nano2sec(virtualFrameTime);
}
f64 game_clock::get_physics_time() const noexcept
{
    return nano2sec(physicsLastFrameTime);
}
f64 game_clock::get_delta() const noexcept
{
    return nano2sec(deltaTime);
}
f64 game_clock::get_real_delta() const noexcept
{
    return nano2sec(realDeltaTime);
}
f64 game_clock::get_physics_delta() const noexcept
{
    return nano2sec(physicsDeltaTime);
}
f64 game_clock::get_alpha() const noexcept
{
    return alpha;
}

} // namespace lemon
