#pragma once

#include "../basic_types.h"
#include "../defines.h"
#include "../service.h"

#include <chrono>

namespace lemon {
class service_registry;
class LEMON_PUBLIC game_clock : public service
{
  public:
    LEMON_REGISTER_SERVICE(game_clock);
    using self_type     = game_clock;
    using clock_type    = std::chrono::high_resolution_clock;
    using duration_type = std::chrono::duration<i64, std::nano>;
    using time_point    = std::chrono::time_point<clock_type, duration_type>;

  public:
    game_clock(service_registry&);
    ~game_clock();
    time_point get_real_time() const noexcept
    {
        return realLastFrameTime;
    }
    time_point get_time() const noexcept
    {
        return virtualFrameTime;
    }
    duration_type elapsed_from_start(const time_point& tp) const noexcept;
    void update() noexcept;
    void set_timescale(f32 timeScale) noexcept;
    [[nodiscard]] f32 delta_time() const noexcept
    {
        return deltaTime;
    }
    [[nodiscard]] f32 read_delta_time() const noexcept
    {
        return realDeltaTime;
    }

  private:
    time_point startTime;
    time_point virtualFrameTime;
    time_point realLastFrameTime;
    f32 deltaTime;
    f32 realDeltaTime;
    f32 timeScale;
};
} // namespace lemon
