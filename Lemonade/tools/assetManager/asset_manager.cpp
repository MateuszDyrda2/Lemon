#include "asset_manager.h"

#include <filesystem>
#include <fstream>
#include <rapidxml_ext.h>
#include <unordered_map>
#include <vector>

using namespace rapidxml;
using namespace std;
namespace fs = filesystem;

bool refresh_directory(const std::string& p)
{
    unordered_map<string, string> assets;

    const fs::path _path{ p };
    for(const auto& entry : fs::recursive_directory_iterator(_path))
    {
        if(!entry.is_directory())
            assets[entry.path().stem()] = entry.path();
    }

    xml_document<> doc;
    xml_node<>* decl = doc.allocate_node(node_declaration);
    // adding attributes at the top of xml
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
    doc.append_node(decl);
    // create a pointer to the root node
    xml_node<>* rootNode = doc.allocate_node(node_element, "Assets");

    // iterate through list
    for(const auto& [name, path] : assets)
    {
        xml_node<>* asset = doc.allocate_node(node_element, "Asset");
        char *node_name   = doc.allocate_string(name.c_str()),
             *node_path   = doc.allocate_string(path.c_str());
        asset->append_attribute(doc.allocate_attribute("name", node_name));
        asset->append_attribute(doc.allocate_attribute("path", node_path));
        doc.append_node(asset);
    }
    doc.append_node(rootNode);
    ofstream file("", ios::binary);
    file << doc;
    file.close();
    doc.clear();
}