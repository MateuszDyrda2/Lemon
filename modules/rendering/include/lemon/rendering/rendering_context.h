#pragma once

#include <lemon/core/basic_types.h> // for size_type
#include <lemon/core/defines.h>
#include <lemon/core/math/color.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec4.h>

#include <glad/glad.h> // GLenum

namespace lemon {
/** OpenGL rendering context */
class LEMON_PUBLIC rendering_context
{
  public:
    rendering_context();
    ~rendering_context();
    /** @brief Enable blending and set the blend function */
    static void enable_blending();
    /** @brief Set new viewport
     * @param vp vector4 to set as a viewport (x, y, width, height)
     */
    static void set_viewport(const ivec4& vp);
    /** @brief Clear screen with a color
     * @param color (r, g, b, a)
     */
    static void clear_screen(const color& c);
    /** @brief Render primitives from arrays data
     * @param mode kind of primitives to render
     * @param first starting index in the enabled arrays
     * @param count number of indices to be rendered
     */
    static void draw_arrays(GLenum mode, size_type first, size_type count);
    /** @brief Render primitives from array data
     * @param mode kind of primitives to renderer
     * @param count number of elements to be rendered
     * @param type type of values in indices
     * @param indices pointer to the location where the indices are stored
     */
    static void draw_elements(GLenum mode, size_type count, GLenum type, void* indices);
    /** @brief Render primitives from array data
     * @param mode kind of primitives to render
     * @param first starting index in the enabled arrays
     * @param count number of elements to be rendered
     * @param instanceCount number of instances to be rendered
     */
    static void draw_arrays_instanced(GLenum mode, size_type first, size_type count,
                                      size_type instanceCount);
    /** @brief Render primitives from array data
     * @param mode kind of primitives to render
     * @param count number of elements to be rendered
     * @param type of values in indices
     * @param pointer to the location where the indices are stored
     * @param instanceCount number of instances to be rendered
     */
    static void draw_elements_instanced(GLenum mode, size_type count, GLenum type,
                                        const void* indices, size_type instanceCount);

  private:
    /** @brief Initializes the rendering context */
    static void create();
    /** @brief Destroys the rendering context */
    static void drop();
};
} // namespace lemon
