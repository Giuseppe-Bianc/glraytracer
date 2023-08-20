#include "VAO.h"

VAO::VAO() noexcept { glGenVertexArrays(1, &ID); }

// Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride,
                     const void *offset) const noexcept {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, gsl::narrow_cast<GLsizei>(stride), offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() const noexcept { glBindVertexArray(ID); }

// Unbinds the VAO
void VAO::Unbind() const noexcept { glBindVertexArray(0); }

// Deletes the VAO
void VAO::Delete() const noexcept { glDeleteVertexArrays(1, &ID); }
