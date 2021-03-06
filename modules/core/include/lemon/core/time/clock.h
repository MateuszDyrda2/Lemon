#pragma once

#include "../basic_types.h"
#include "../defines.h"

#include <chrono>

namespace lemon {
class LEMON_PUBLIC clock
{
  public:
    using self_type     = clock;
    using clock_type    = std::chrono::high_resolution_clock;
    using duration_type = std::chrono::duration<i64, std::nano>;
    using time_point    = std::chrono::time_point<clock_type, duration_type>;

  public:
    clock();
    ~clock();
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
