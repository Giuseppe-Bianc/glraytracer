#pragma once
#include "headers.h"

class VBO {
public:
    // Reference ID of the Vertex Buffer Object
    GLuint ID{};
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(GLdouble *vertices, GLsizeiptr size) noexcept;

    // Binds the VBO
    void Bind() const noexcept;
    // Unbinds the VBO
    void Unbind() const noexcept;
    // Deletes the VBO
    void Delete() const noexcept;
};
