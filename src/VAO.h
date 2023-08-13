#pragma once
#include "VBO.h"

class VAO {
public:
    // ID reference for the Vertex Array Object
    GLuint ID{};
    // Constructor that generates a VAO ID
    VAO() noexcept;

    // Links a VBO to the VAO using a certain layout
    void LinkVBO(VBO &VBO, GLuint layout) noexcept;
    // Binds the VAO
    void Bind() noexcept;
    // Unbinds the VAO
    void Unbind() noexcept;
    // Deletes the VAO
    void Delete() noexcept;
};
