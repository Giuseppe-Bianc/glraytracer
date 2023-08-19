#pragma once

#include <glad/glad.h>

class EBO {
public:
    // ID reference of Elements Buffer Object
    GLuint ID{};
    // Constructor that generates a Elements Buffer Object and links it to indices
    EBO(const GLuint *indices, GLsizeiptr size) noexcept;

    // Binds the EBO
    void Bind() noexcept;
    // Unbinds the EBO
    void Unbind() noexcept;
    // Deletes the EBO
    void Delete() noexcept;
};
