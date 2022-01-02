#pragma once

#include "service.h"

#include <river/core/basic_types.h>

#include <vector>

namespace river {
class services
{
  public:
    template<class T>
    static ptr<T> get()
    {
        return static_cast<ptr<T>>(serviceArray[get_type_index<std::decay_t<T>>()].get());
    }

  private:
    friend class engine;
    services() = default;
    template<class T>
    static void provide(owned<T>&& serv)
    {
        get_type_index<std::decay_t<T>>();
        serviceArray.push_back(std::move(serv));
    }

  private:
    static std::vector<owned<service>> serviceArray;

    static size_type get_index()
    {
        static size_type index = 0;
        return index++;
    }
    template<class T>
    inline static size_type get_type_index()
    {
        static size_type index = get_index();
        return index;
    }

  public:
    services(const services&) = delete;
    services& operator=(const services&) = delete;
};
}
