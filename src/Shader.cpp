#include "Shader.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char *filename) {
    if(std::ifstream in(filename, std::ios::binary); in.is_open()) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        return contents;
    }
    throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    // Read vertexFile and fragmentFile and store the strings
    std::string vertexCode;
    std::string fragmentCode;
    try {
        vertexCode = get_file_contents(vertexFile);
        fragmentCode = get_file_contents(fragmentFile);
    } catch(const std::exception &e) {
        LERROR("expetion {}", e.what());
    }

    // Convert the shader source strings into character arrays
    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    // Create Vertex Shader Object and get its reference
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);

    // Create Fragment Shader Object and get its reference
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    // Compile the Vertex Shader into machine code
    glCompileShader(fragmentShader);

    // Create Shader Program Object and get its reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(ID);

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::Activate() noexcept { glUseProgram(ID); }

// Deletes the Shader Program
void Shader::Delete() noexcept  { glDeleteProgram(ID); }
