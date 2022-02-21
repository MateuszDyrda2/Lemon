#include <lemon/assets/file.h>

namespace lemon {
file::file():
    stream()
{
}
file::file(const std::string& path, openmode flag):
    stream(path, flag)
{
}
file::~file()
{
}
void file::open(const std::string& path, openmode flag)
{
    stream.open(path, flag);
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
