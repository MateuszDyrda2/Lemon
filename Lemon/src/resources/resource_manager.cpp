#include <lemon/resources/resource_manager.h>

#include <lemon/core/logger.h>

namespace lemon {
resource_manager::resource_manager():
    resources{}
{
    LOG_MESSAGE("Resource Manager created");
}
resource_manager::~resource_manager()
{
    LOG_MESSAGE("Resource Manager destroyed");
}
void resource_manager::unload(string_id name)
{
    resources.erase(name);
}
} // namespace lemon
