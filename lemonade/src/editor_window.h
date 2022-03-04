#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <lemon/window/window_base.h>

using namespace lemon;
class editor_window : public window_base
{
public:
    editor_window(size_type width, size_type height);
    virtual ~editor_window();

    bool end_frame() override;
    void* get_handle() override;

};

#endif // EDITOR_WINDOW_H
