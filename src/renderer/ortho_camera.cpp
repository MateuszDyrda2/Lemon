#include <glm/gtx/transform.hpp>
#include <river/renderer/ortho_camera.h>

namespace river {
void ortho_camera::recalculate_projection()
{
    projection = glm::ortho(viewport.x, viewport.z, viewport.y, viewport.w);
}
} // namespace river
