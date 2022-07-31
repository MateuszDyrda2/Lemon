#pragma once

#include <lemon/assets/asset.h>
#include <lemon/events/events.h>
#include <lemon/platform/input.h>
#include <lemon/platform/window.h>
#include <lemon/rendering/rendering_context.h>
#include <lemon/threading/scheduler.h>
#include <lemon/scripting/domain.h>

namespace lemon {
class engine
{
  public:
    engine(int argc, char** argv, const std::string& assetPath);
    virtual ~engine();
    int run();

  private:
    event_queue events;
    window w;
    input in;
    rendering_context rContext;
    asset_storage assets;
    scheduler sch;
	domain scripting;
};
} // namespace lemon
