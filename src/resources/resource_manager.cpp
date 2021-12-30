#include <river/resources/resource_manager.h>

namespace river {
resource_manager::resource_manager()
{
}
resource_manager::~resource_manager()
{
}
resource_manager& resource_manager::get()
{
    static resource_manager instance;
    return instance;
}
} // namespace river
