#include <lemon/assets/asset_loader.h>

#include <lemon/core/logger.h>

#include <rapidjson/document.h>

#include <fstream>

namespace lemon {
using namespace std;
using namespace rapidjson;
asset_loader::asset_loader(const string& path):
    path(path)
{
    using namespace rapidjson;
    ifstream stream(path, ios::binary | ios::ate);
    if(!stream)
    {
        logger::fatal("Failed to open asset file: {}", path);
        return;
    }
    std::size_t nBytes = stream.tellg();
    stream.seekg(0, stream.beg);
    vector<char> buff;
    buff.resize(nBytes + 1ULL);
    stream.read(buff.data(), nBytes);
    stream.close();

    buff.push_back('\0');
    Document document;
    document.ParseInsitu(buff.data());

    for(auto a = document.Begin(); a != document.End(); ++a)
    {
        auto iter = a->MemberBegin();
        auto name = iter->value.GetString();
        ++iter;
        auto path                          = iter->value.GetString();
        resourcePaths[hash_string_d(name)] = path;
    }
}
asset_loader::~asset_loader()
{
}
bool asset_loader::resource_exists(hash_str name) const noexcept
{
    return resourcePaths.contains(name);
}
std::vector<char> asset_loader::load_from_file(const string& path)
{
    vector<char> buff;
    ifstream stream(path, ios::binary | ios::ate);
    if(stream)
    {
        std::size_t nBytes = stream.tellg();
        stream.seekg(0, stream.beg);
        buff.reserve(nBytes + 1ULL);
        stream.read(buff.data(), nBytes);
        stream.close();
    }
    return buff;
}
} // namespace lemon
