#pragma once

#include <lemon/containers/buffer.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

#include <fstream>
#include <iterator>
#include <vector>

namespace lemon {
class LEMON_PUBLIC file
{
  public:
    using openmode = std::ios::openmode;

  public:
    file();
    file(const std::string& path, openmode flag);
    ~file();
    void open(const std::string& path, openmode flag);
    void close();
    template<class T>
    void read(buffer<T>& buff);
    template<class T>
    void read(std::vector<T>& buff);

    template<class T>
    void write(const std::vector<T>& buff);
    void write(const std::string& buff);
    void write(const char* buff, size_type length);

  private:
    std::fstream stream;
};
template<class T>
void file::read(buffer<T>& buff)
{
    stream.seekg(0, stream.end);
    size_type size = stream.tellg();
    stream.seekg(0, stream.beg);

    buff = buffer<T>(size);
    stream.read((char*)buff.get(), size);
}
template<class T>
void file::read(std::vector<T>& buff)
{
    stream.seekg(0, stream.end);
    size_type size = stream.tellg();
    stream.seekg(0, stream.beg);

    buff.resize(size);
    stream.read((char*)buff.data(), size);
}
template<class T>
void file::write(const std::vector<T>& buff)
{
    stream.write((char*)buff.data(), buff.size());
}
} // namespace lemon
