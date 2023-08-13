#include "VAO.h"

VAO::VAO() noexcept { glGenVertexArrays(1, &ID); }

// Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride,const void *offset) noexcept {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() noexcept { glBindVertexArray(ID); }

// Unbinds the VAO
void VAO::Unbind() noexcept { glBindVertexArray(0); }

// Deletes the VAO
void VAO::Delete() noexcept { glDeleteVertexArrays(1, &ID); }
