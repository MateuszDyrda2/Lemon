#include "editor_window.h"

editor_window::editor_window(size_type width, size_type height) :
    base_window(width, height)
{

}
editor_window::~editor_window()
{

}
bool editor_window::end_frame()
{
    return false;
}
void* editor_window::get_handle()
{
    return nullptr;
}
