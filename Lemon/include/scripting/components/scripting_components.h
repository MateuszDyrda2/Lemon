#pragma once

#include "../script.h"

#include <assets/asset.h>
#include <core/defines.h>
#include <world/reflection.h>

namespace lemon {
struct LEMON_API [[lemon::component]] script_component
{
    asset<script> scriptObject;
};
}
