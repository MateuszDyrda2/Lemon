#include <lemon/assets/file.h>

namespace lemon {
using namespace std;
file::file():
    stream()
{
}
file::file(const string& path, openmode flag):
    stream(path, flag)
{
}
file::~file()
{
}
void file::open(const string& path, openmode flag)
{
    stream.exceptions(fstream::badbit);
    try
    {
        stream.open(path, flag);
    }
    catch(const fstream::failure& e)
    {
        LOG_ERROR("Failed while trying to open %s with %s", path, e.what());
    }
}
void file::close()
{
    stream.close();
}
void file::write(const std::string& buff)
{
    stream.write(buff.c_str(), buff.size());
}
void file::write(const char* buff, size_type length)
{
    stream.write(buff, length);
}
} // namespace lemon
