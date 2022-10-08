#include <engine/engine.h>

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
    while(_window.update())
    {
        _input.update();
        _eventQueue.process();
        _sceneManager.update();
        _assertStorage.update();
    }
    return 0;
}
} // namespace lemon
