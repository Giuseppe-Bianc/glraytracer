#pragma once

#include "headers.h"
#include <stb_image.h>

#include "Shader.h"

class Texture {
public:
    GLuint ID{};
    GLenum type{};
    Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) noexcept;

    // Assigns a texture unit to a texture
    void texUnit(Shader &shader, const char *uniform, GLuint unit) const noexcept;
    // Binds a texture
    void Bind() const noexcept;
    // Unbinds a texture
    void Unbind() const noexcept;
    // Deletes a texture
    void Delete() const noexcept;
};
