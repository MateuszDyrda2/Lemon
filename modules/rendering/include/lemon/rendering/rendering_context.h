#pragma once

#include <lemon/core/lemon_types.h>
#include <lemon/core/math/color.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec4.h>

#include <glad/glad.h> // GLenum

namespace lemon {
/** OpenGL rendering context */
class rendering_context
{
  public:
    /** @brief Initializes the rendering context */
    rendering_context();
    /** @brief Destroys the rendering context */
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
    static void draw_arrays(GLenum mode, std::size_t first, std::size_t count);
    /** @brief Render primitives from array data
     * @param mode kind of primitives to renderer
     * @param count number of elements to be rendered
     * @param type type of values in indices
     * @param indices pointer to the location where the indices are stored
     */
    static void draw_elements(GLenum mode, std::size_t count, GLenum type, void* indices);
    /** @brief Render primitives from array data
     * @param mode kind of primitives to render
     * @param first starting index in the enabled arrays
     * @param count number of elements to be rendered
     * @param instanceCount number of instances to be rendered
     */
    static void draw_arrays_instanced(GLenum mode, std::size_t first, std::size_t count,
                                      std::size_t instanceCount);
    /** @brief Render primitives from array data
     * @param mode kind of primitives to render
     * @param count number of elements to be rendered
     * @param type of values in indices
     * @param pointer to the location where the indices are stored
     * @param instanceCount number of instances to be rendered
     */
    static void draw_elements_instanced(GLenum mode, std::size_t count, GLenum type,
                                        const void* indices, std::size_t instanceCount);
};
} // namespace lemon
