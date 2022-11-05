#include "world/system.h"
#include <engine/engine.h>

#include <chrono>

namespace lemon {
engine::engine(int /* argc*/, char** /*argv*/, const std::string& assetPath):
    _eventQueue{}, _window("engine", { 1080, 720 }),
    _input(_window, _eventQueue), _renderingContext{},
    _assertStorage(assetPath, _scriptingEngine),
    _scheduler(std::thread::hardware_concurrency() - 1),
    _sceneManager(_assertStorage, _scheduler, _eventQueue, _window, _input, _messageBus)
{ }

engine::~engine()
{
    logger::info("Engine has finished working");
}

int engine::run()
{
    using namespace std::chrono;

    auto lastFrame  = high_resolution_clock::now();
    f32 delta       = 0.016,
        fixedDelta  = 0.020f,
        accumulator = {},
        alpha       = {};

    while (_window.update())
    {
        auto currentFrame = high_resolution_clock::now();
        delta =
            duration_cast<duration<f32>>(
                currentFrame - lastFrame)
                .count();
        lastFrame = currentFrame;

        // poll current input state
        _input.update();
        // run all pending events
        _eventQueue.process();
        // run update
        update_event eu(delta, alpha);
        _eventQueue["EarlyUpdate"_hs]
            .fire_immediate(&eu);

        accumulator += delta;
        while (accumulator >= fixedDelta)
        {
            fixed_update_event fe(fixedDelta);
            _eventQueue["PhysicsUpdate"_hs]
                .fire_immediate(&fe);
            accumulator -= fixedDelta;
        }
        alpha = accumulator / fixedDelta;
        update_event e(delta, alpha);
        _eventQueue["Update"_hs]
            .fire_immediate(&e);
        _eventQueue["Render"_hs]
            .fire_immediate(&e);
        // clean unused assets
        _assertStorage.update();
    }
    return 0;
}
} // namespace lemon
