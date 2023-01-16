#pragma once

using namespace lemon;
struct [[lemon::component]] player_t
{ };

struct [[lemon::component]] enemy_component
{
    [[lemon::field]] f32 life;
};

struct [[lemon::component]] fireball_t
{ };
