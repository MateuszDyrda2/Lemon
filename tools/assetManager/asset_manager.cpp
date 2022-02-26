#include "asset_manager.h"

#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace rapidjson;
using namespace std;
namespace fs = filesystem;

bool refresh_directory(const std::string& assetFile, const std::string& directory)
{
    unordered_map<string, string> assets;

    const fs::path _path{ directory };
    for(const auto& entry : fs::recursive_directory_iterator(_path))
    {
        if(!entry.is_directory())
            assets[entry.path().stem().string()] = entry.path().string();
    }

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartArray();
    {
        for(const auto& [name, path] : assets)
        {
            writer.StartObject();
            {
                writer.Key("name");
                writer.String(name.c_str());
                writer.Key("path");
                writer.String(path.c_str());
            }
            writer.EndObject();
        }
    }
    writer.EndArray();
    ofstream file;
    file.open(assetFile, ios::binary);
    file.write(s.GetString(), s.GetLength());
    file.close();
}