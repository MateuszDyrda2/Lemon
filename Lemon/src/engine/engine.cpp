#include "world/system.h"
#include <engine/engine.h>

#include <chrono>

namespace lemon {
engine::engine(int /* argc*/, char** /*argv*/, const std::string& assetPath):
    _eventQueue{}, _window("engine", { 1080, 720 }),
    _input(_window, _eventQueue), _renderingContext{},
    _assertStorage(assetPath),
    _scheduler(std::thread::hardware_concurrency() - 1),
    _sceneManager(_assertStorage, _scheduler, _eventQueue, _window, _input)
{ }

engine::~engine()
{
    logger::info("Engine has finished working");
}

int engine::run()
{
    using namespace std::chrono;
    auto lastFrame = high_resolution_clock::now();

    while(_window.update())
    {
        auto currentFrame = high_resolution_clock::now();
        auto deltaTime    = duration_cast<duration<f32>>(currentFrame - lastFrame).count();
        lastFrame         = currentFrame;
        // poll current input state
        _input.update();
        // run all pending events
        _eventQueue["EarlyUpdate"_hs].fire(new update_event(deltaTime));
        _eventQueue["Update"_hs].fire(new update_event(deltaTime));
        _eventQueue["LateUpdate"_hs].fire(new update_event(deltaTime));
        _eventQueue["Render"_hs].fire(new update_event(deltaTime));
        _eventQueue.process();
        // clean unused assets
        _assertStorage.update();
    }
    return 0;
}
} // namespace lemon
