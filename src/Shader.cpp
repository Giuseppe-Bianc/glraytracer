#include "Shader.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char *filname) {
    if(std::ifstream in(filname, std::ios::binary); in.is_open()) {
        return std::string(std::istreambuf_iterator<char>(in), {});
    }
    char errMessage[256];
    strerror_s(errMessage, sizeof(errMessage), errno);
    throw std::runtime_error(errMessage);
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
    // Checks if Shader compiled succesfully
    compileErrors(vertexShader, "VERTEX");

    // Create Fragment Shader Object and get its reference
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    // Compile the Vertex Shader into machine code
    glCompileShader(fragmentShader);
    // Checks if Shader compiled succesfully
    compileErrors(fragmentShader, "FRAGMENT");

    // Create Shader Program Object and get its reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(ID);
    // Checks if Shaders linked succesfully
    compileErrors(ID, "PROGRAM");

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::Activate() const noexcept { glUseProgram(ID); }

// Deletes the Shader Program
void Shader::Delete() const noexcept { glDeleteProgram(ID); }

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, std::string_view type) const {
    // Stores status of compilation
    GLint hasCompiled;
    // Character array to store error message in
    std::array<char, 1024> infoLog{};
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, gsl::narrow_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data());
            LERROR("SHADER_COMPILATION_ERROR for: {}", type);
            LERROR("{}", infoLog.data());
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE) {
            LERROR("SHADER_COMPILATION_ERROR for: {}", type);
            LERROR("{}", infoLog.data());
        }
    }
}
