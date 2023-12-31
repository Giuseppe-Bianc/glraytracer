#pragma once
#include "headers.h"
std::string get_file_contents(const char *filname);

class Shader {
public:
    // Reference ID of the Shader Program
    GLuint ID{};
    // Constructor that build the Shader Program from 2 different shaders
    Shader(const char *vertexFile, const char *fragmentFile);

    // Activates the Shader Program
    void Activate() const noexcept;
    // Deletes the Shader Program
    void Delete() const noexcept;

private:
    // Checks if the different Shaders have compiled properly
    void compileErrors(unsigned int shader, std::string_view type) const;
};
