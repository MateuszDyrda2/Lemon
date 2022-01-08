#include <lemon/resources/resource_manager.h>

#include <lemon/core/logger.h>
#include <lemon/job_system/job_system.h>
#include <lemon/service/services.h>

#include <fstream>

namespace lemon {
resource_manager::resource_manager():
    resources{}, jobSystem(services::get<job_system>())
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
buffer<byte> resource_manager::read_file(const std::string& path)
{
    std::ifstream file;
    buffer<byte> b;
    file.exceptions(std::ofstream::badbit);
    try
    {
        file.open(path, std::ios::binary);
        file.seekg(std::ios::end);
        size_type size = file.tellg();
        file.seekg(std::ios::beg);
        b = buffer<byte>(size);
        file.read((char*)b.get(), size);
    }
    catch(const std::exception& err)
    {
        LOG_ERROR("Failed to load asset from %s, exception returned: %s",
                  path.c_str(), err.what());
    }
    return b;
}
resource_manager::future_type<buffer<byte>>
resource_manager::read_async(const std::string& path)
{
    return jobSystem->schedule_r([=]() -> buffer<byte> {
        std::ifstream file;
        buffer<byte> b;
        file.open(path, std::ios::binary);
        file.seekg(std::ios::end);
        size_type size = file.tellg();
        file.seekg(std::ios::beg);
        b = buffer<byte>(size);
        file.read((char*)b.get(), size);
        return b;
    });
}
} // namespace lemon
