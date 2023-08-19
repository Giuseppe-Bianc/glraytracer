#pragma once
#include "VBO.h"

class VAO {
public:
    // ID reference for the Vertex Array Object
    GLuint ID{};
    // Constructor that generates a VAO ID
    VAO() noexcept;

    // Links a VBO Attribute such as a position or color to the VAO
    void LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride,
                    const void *offset) const noexcept;
    // Binds the VAO
    void Bind() noexcept;
    // Unbinds the VAO
    void Unbind() noexcept;
    // Deletes the VAO
    void Delete() noexcept;
};
