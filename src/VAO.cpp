#include "VAO.h"

VAO::VAO() noexcept { glGenVertexArrays(1, &ID); }

// Links a VBO to the VAO using a certain layout
void VAO::LinkVBO(VBO &VBO, GLuint layout) noexcept {
    VBO.Bind();
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() noexcept { glBindVertexArray(ID); }

// Unbinds the VAO
void VAO::Unbind() noexcept { glBindVertexArray(0); }

// Deletes the VAO
void VAO::Delete() noexcept { glDeleteVertexArrays(1, &ID); }
