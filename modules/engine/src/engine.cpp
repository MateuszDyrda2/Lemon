#include <lemon/engine/engine.h>

#include <lemon/core/hash_string.h>
#include <lemon/core/lemon_types.h>
#include <lemon/core/logger.h>

namespace lemon {
engine::engine(int argc, char** argv, const std::string& assetPath):
    events{}, w("engine", { 1080, 720 }),
    in(w, events), rContext{},
    assets(assetPath),
    sch(std::thread::hardware_concurrency() - 1)
{
}
engine::~engine()
{
    logger::info("Engine has finished working");
}
int engine::run()
{
    while(w.update())
    {
        in.update();
        events.process();
        assets.update();
    }
    return 0;
}
} // namespace lemon
