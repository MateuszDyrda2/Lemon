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
    using self_type  = game_clock;
    using clock_type = std::chrono::high_resolution_clock;

  public:
    game_clock(service_registry&);
    ~game_clock();
    void calculate_delta_time() noexcept;
    void set_timescale(f32 timeScale) noexcept;

    [[nodiscard]] f64 get_real_time() const noexcept;
    [[nodiscard]] f64 get_time() const noexcept;
    [[nodiscard]] f64 get_physics_time() const noexcept;

    [[nodiscard]] f64 get_delta() const noexcept;
    [[nodiscard]] f64 get_real_delta() const noexcept;
    [[nodiscard]] f64 get_physics_delta() const noexcept;
    [[nodiscard]] f64 get_alpha() const noexcept;

    template<class F>
    void physics_step(F&& callable);

  private:
    clock_type::time_point startTime;
    clock_type::time_point realLastFrameTime;
    i64 virtualFrameTime;
    i64 physicsLastFrameTime;
    i64 deltaTime;
    i64 physicsDeltaTime;
    i64 realDeltaTime;
    f32 timeScale;

    f64 accumulator;
    f64 alpha;

  private:
    static inline f64 nano2sec(i64 val)
    {
        return f64(val * 1'000'000'000.0);
    }
};
template<class F>
void game_clock::physics_step(F&& callable)
{
    accumulator += get_delta();
    auto pdt = get_physics_delta();
    while(accumulator >= pdt)
    {
        callable();
        physicsLastFrameTime += physicsDeltaTime;
        accumulator -= pdt;
    }
    alpha = accumulator / pdt;
}
} // namespace lemon
