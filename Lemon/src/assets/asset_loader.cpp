#include <lemon/assets/asset_loader.h>

#include <lemon/assets/file.h>

#include <rapidxml.hpp>

namespace lemon {
asset_loader::asset_loader(const std::string& dataPath)
{
    using namespace rapidxml;

    file dataFile(dataPath, std::ios::binary | std::ios::in);
    std::vector<char> buff;
    dataFile.read(buff);
    dataFile.close();
    buff.push_back('\0');
    xml_document<> doc;
    doc.parse<0>(buff.data());
    xml_node<>* rootNode = doc.first_node("Assets");
    for(auto assetNode = rootNode->first_node("Asset"); assetNode;
        assetNode      = assetNode->next_sibling())
    {
        auto nameAttr = assetNode->first_attribute("name");
        auto pathAttr = assetNode->first_attribute("path");
        string_id name(nameAttr->value());
        std::string path(pathAttr->value(), pathAttr->value_size());
        resourcePaths.insert(std::make_pair(name, path));
    }
}
asset_loader::~asset_loader()
{
}
bool asset_loader::resource_exists(string_id name) const noexcept
{
    return resourcePaths.contains(name);
}
std::vector<byte> asset_loader::load_from_file(const std::string& path)
{
    file dataFile(path, std::ios::binary | std::ios::in);
    std::vector<byte> buff;
    dataFile.read(buff);
    dataFile.close();
    return buff;
}
} // namespace lemon
