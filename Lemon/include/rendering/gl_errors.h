/** @file gl_errors.h
 * @brief File with helper function definitions for opengl
 */
#pragma once

#include <core/logger.h>
#include <glad/glad.h>

namespace lemon::detail {
inline void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    auto err = glGetError();
    if (err == GL_NO_ERROR) return;

    logger::error("OpenGL error: {:#x}, at {}:{} for {}", err, fname, line, stmt);
}
}

#ifdef LEMON_DEBUG
#    define GL_CHECK(stmt)                                       \
        do {                                                     \
            stmt;                                                \
            detail::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        }                                                        \
        while (0)
#else
#    define GL_CHECK(stmt) stmt
#endif
