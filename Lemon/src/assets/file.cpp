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
} // namespace lemon
