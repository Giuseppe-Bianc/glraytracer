#include "EBO.h"
#include "Shader.h"
#include "Timer.h"
#include "VAO.h"
#include "VBO.h"
#include "headers.h"

// Vertices coordinates
std::array<GLfloat, 18> vertices = {
    -0.5f,  -0.288675135f, 0.0f,  // Lower left corner
    0.5f,   -0.288675135f, 0.0f,  // Lower right corner
    0.0f,   0.57735027f,   0.0f,  // Upper corner
    -0.25f, 0.144337568f,  0.0f,  // Inner left
    0.25f,  0.144337568f,  0.0f,  // Inner right
    0.0f,   -0.288675135f, 0.0f   // Inner down
};

// Indices for vertices order
std::array<GLuint, 9> indices = {
    0, 3, 5,  // Lower left triangle
    3, 2, 4,  // Lower right triangle
    5, 4, 1   // Upper triangle
};

void errorCallback(int error, const char *description) { GLWFERR(error, description) }

void framebufferSizeCallback([[maybe_unused]] GLFWwindow *window, int width, int height) noexcept { glViewport(0, 0, width, height); }

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

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("./src/default.vert", "./src/default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices.data(), sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices.data(), sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkVBO(VBO1, 0);
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    timer3.stop();
    const double shadersetup = timer3.elapsedSeconds();
    LINFO("shaders,vao evbo setup eseguito in {:f}", shadersetup);
    const double totalT = totalw + shadersetup;
    LINFO("total setup time {:f}", totalT);

    // Main while loop
    while(!glfwWindowShouldClose(window)) {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
