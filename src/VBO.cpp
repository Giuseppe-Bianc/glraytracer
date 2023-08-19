#include "VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLdouble *vertices, GLsizeiptr size) noexcept {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::Bind() const noexcept { glBindBuffer(GL_ARRAY_BUFFER, ID); }

// Unbinds the VBO
void VBO::Unbind() const noexcept { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// Deletes the VBO
void VBO::Delete() const noexcept { glDeleteBuffers(1, &ID); }
