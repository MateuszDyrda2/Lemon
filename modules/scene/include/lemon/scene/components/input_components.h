#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/scene/reflection.h>

#include <lemon/platform/input.h>

namespace lemon {
struct LEMON_PUBLIC input_component : public component
{
    input& inputManager = *input::inputManager;
    input_component()   = default;
    ~input_component()  = default;

    LEMON_REFLECT(input_component);
};
} // namespace lemon
