/** @file rendering_system.h
 * @brief File with a rendering system definition
 */
#pragma once

#include "../basic_renderer.h"
#include "../components/rendering_components.h"

#include <assets/asset.h>
#include <events/events.h>
#include <world/components/transform_components.h>
#include <world/system.h>

namespace lemon {
/** Rendering system */
class LEMON_API rendering_system : public system
{
  public:
    SYSTEM(rendering_system, scene, event_queue, asset_storage, window);
    ~rendering_system();
    /** @brief Function that runs on each rerender
     * @param e event arguments
     */
    void on_render(event_args* e);

  private:
    scene& _scene;

    event_queue::listener_handle update;
    event_queue::listener_handle framebufferSize;
    basic_renderer renderer;
    entity_t mainCamera;
    ivec2 viewport;
};
}
