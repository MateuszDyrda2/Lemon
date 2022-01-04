#include <lemon/service/services.h>

namespace lemon {
std::vector<owned<service>> services::serviceArray;

void services::cleanup()
{
    auto size = serviceArray.size();
    for(size_type i = 0; i < size; ++i)
        serviceArray.pop_back();
}
} // namespace lemon
