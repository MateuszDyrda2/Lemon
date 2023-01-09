/** @file scripting components
 * @brief Contains definitons of components used with scripting
 */
#pragma once

#include "../script.h"
#include <assets/asset.h>
#include <core/defines.h>

namespace lemon {
/** Script component */
struct LEMON_API [[lemon::component]] script_component
{
    [[lemon::field]] asset<script> scriptObject;
};
}
