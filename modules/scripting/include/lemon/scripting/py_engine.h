#pragma once

#include <lemon/core/defines.h>
#include <lemon/core/logger.h>

#include <pybind11/embed.h>

namespace lemon {
namespace py = pybind11;
class LEMON_PUBLIC py_engine
{
  public:
    py_engine();
    ~py_engine();

  private:
    py::scoped_interpreter _interpreter;
};
} // namespace lemon
