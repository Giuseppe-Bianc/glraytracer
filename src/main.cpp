#include "EBO.h"
#include "Shader.h"
#include "Timer.h"
#include "VAO.h"
#include "VBO.h"
#include "headers.h"

constexpr std::array<GLfloat, 18> vertices = {
    -0.5f,  -0.288675135f, 0.0f,  // Lower left corner
    0.5f,   -0.288675135f, 0.0f,  // Lower right corner
    0.0f,   0.57735027f,   0.0f,  // Upper corner
    -0.25f, 0.144337568f,  0.0f,  // Inner left
    0.25f,  0.144337568f,  0.0f,  // Inner right
    0.0f,   -0.288675135f, 0.0f   // Inner down
};

constexpr std::array<GLuint, 9> indices = {
    0, 3, 5,  // Lower left triangle
    3, 2, 4,  // Upper triangle
    5, 4, 1   // Lower right triangle
};

// Vertex Shader source code
constexpr const char *vertexShaderSource = R"(
#version 460 core
#extension GL_ABC_foobar_feature : enable
#extension GL_ARB_gpu_shader_fp64 : enable
layout (location = 0) in vec3 aPos;
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

// Fragment Shader source code
constexpr const char *fragmentShaderSource = R"(
#version 460 core
#extension GL_ABC_foobar_feature : enable
#extension GL_ARB_gpu_shader_fp64 : enable
out vec4 FragColor;
void main(){
    FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);
}
)";

void errorCallback(int error, const char *description) { GLWFERR(error, description) }

void framebufferSizeCallback([[maybe_unused]] GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) noexcept {
    switch(key) {
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    // Add more cases for other keys if needed
    default:
        // Handle other keys here
        break;
    }
}

int main() {
    spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console);
    GLFWwindow *window = nullptr;
    Timer timer;
    if(!glfwInit())
        return -1;
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(w, h, windowTitle.data(), nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    if(primaryMonitor == nullptr) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
    if(videoMode == nullptr) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    const int centerX = CALC_CENTRO(videoMode->width, w);
    const int centerY = CALC_CENTRO(videoMode->height, h);

    glfwSetWindowPos(window, centerX, centerY);

    int posX = 0;
    int posY = 0;
    glfwGetWindowPos(window, &posX, &posY);
    if(posX != centerX || posY != centerY) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    timer.stop();
    const double wcreation = timer.elapsedSeconds();
    LINFO("creazione finestra {}: (w: {}, h: {}, pos:(x:{}, y:{})) eseguita in {:f}", windowTitle.data(), w, h, centerX, centerY,
          wcreation);
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);
    Timer timer2;
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LERROR("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, w, h);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    timer2.stop();
    const double glaadsetup = timer2.elapsedSeconds();
    LINFO("glad setup eseguito in {:f}", glaadsetup);
    const double totalw = wcreation + glaadsetup;
    LINFO("total window setup time {:f}", totalw);
    Timer timer3;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    timer3.stop();
    const double shadersetup = timer3.elapsedSeconds();
    LINFO("shaders,vao evbo setup eseguito in {:f}", shadersetup);
    const double totalT = totalw + shadersetup;
    LINFO("total setup time {:f}", totalT);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
