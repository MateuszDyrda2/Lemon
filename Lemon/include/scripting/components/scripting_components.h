#pragma once

#include "../script.h"
#include <assets/asset.h>
#include <core/defines.h>

namespace lemon {
struct LEMON_API [[lemon::component]] script_component
{
    [[lemon::field]] asset<script> scriptObject;
};
}
