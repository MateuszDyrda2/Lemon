#pragma once

#include <lemon/core/string_id.h>
#include <lemon/core/defines.h>
#include <lemon/scene/reflection.h>

namespace lemon {
struct LEMON_PUBLIC destroy_m
{ };
struct LEMON_PUBLIC enable_m
{ };
struct LEMON_PUBLIC disable_m
{ };
struct LEMON_PUBLIC enabled_t
{ };
struct LEMON_PUBLIC tag : public component
{
    string_id id;
	tag() = default;
	tag(string_id id) : id(id) {}
	~tag() = default;

	LEMON_REFLECT(tag, id);
};
} // namespace lemon
