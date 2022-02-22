#include <lemon/assets/asset_loader.h>

#include <lemon/assets/file.h>

#include <rapidjson/document.h>

namespace lemon {
using namespace std;
asset_loader::asset_loader(const std::string& dataPath)
{
    using namespace rapidjson;
    /*
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
    */
    file dataFile;
    dataFile.open(dataPath, ios::binary | ios::in);
    vector<char> buff;
    dataFile.read(buff);
    dataFile.close();
    Document document;
    document.ParseInsitu(buff.data());

    for(auto a = document.Begin(); a != document.End(); ++a)
    {
        auto iter = a->MemberBegin();
        auto name = iter->value.GetString();
        ++iter;
        auto path                      = iter->value.GetString();
        resourcePaths[string_id(name)] = path;
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
