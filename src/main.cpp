#include "EBO.h"
#include "Ray.h"
#include "Shader.h"
#include "Texture.h"
#include "Timer.h"
#include "VAO.h"
#include "VBO.h"
#include "Vec3.h"
#include "headers.h"
#include <stb_image.h>
#include <stb_image_write.h>
// #define LOGING_PROGRESS

// Vertices coordinates

std::array<GLdouble, 32> vertices = {
    //     COORDINATES     /        COLORS      /   TexCoord  //
    -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,  // Lower left corner
    -1.0, 1.0,  0.0, 0.0, 1.0, 0.0, 0.0, 1.0,  // Upper left corner
    1.0,  1.0,  0.0, 0.0, 0.0, 1.0, 1.0, 1.0,  // Upper right corner
    1.0,  -1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0   // Lower right corner
};

std::array<GLuint, 6> indices = {
    0, 2, 1,  // Upper triangle
    0, 3, 2   // Lower triangle
};

void errorCallback(int error, const char *description) { GLWFERR(error, description) }

void framebufferSizeCallback([[maybe_unused]] GLFWwindow *window, int width, int height) noexcept {
    glViewport(0, 0, width, height);
}

/* namespace Detail {
    double constexpr sqrtNewtonRaphson(double x, double curr, double prev) {
        return curr == prev ? curr : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }
}  // namespace Detail

/*
 * Constexpr version of the square root
 * Return value:
 *	- For a finite and non-negative value of "x", returns an approximation for the square root of "x"
 *   - Otherwise, returns NaN
 **
double constexpr sqrtcx(double x) {
    return x >= 0 && x < std::numeric_limits<double>::infinity() ? Detail::sqrtNewtonRaphson(x, x, 0)
                                                                 : std::numeric_limits<double>::quiet_NaN();
}*/

Color ray_color(const ray &r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return 255.0 * ((1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0));
}

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
    int channels = 3;  // RGB channels

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (C_D(w) / h);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / w;
    auto pixel_delta_v = viewport_v / h;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    LINFO("pixel00_loc={}", pixel00_loc.toString());

    // Create a std::vector to hold the image data
    std::vector<unsigned char> image_data(w * h * channels);
    Timer t;
    const std::string_view filename = "./src/output_image.png";
    for(int j = 0; j < h; ++j) {
#ifdef LOGING_PROGRESS
        LINFO("Scanlines remaining: {} ", (h - j));
#endif  // LOGING_PROGRESS
        for(int i = 0; i < w; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            Color pixel_color = ray_color(r);
            const auto index = C_ST((j * w + i) * channels);
            image_data[index + 0] = C_UC(pixel_color.x());  // Red channel
            image_data[index + 1] = C_UC(pixel_color.y());  // Green channel
            image_data[index + 2] = C_UC(pixel_color.z());  // Blue channel
        }
    }
#ifdef LOGING_PROGRESS
    LINFO("Done.");
#endif  // LOGING_PROGRESS
    t.stop();

    LINFO("tempo creazione data immagine {} eseguito in {:f}", filename, t.elapsedSeconds());

    // Write the image data to a file
    stbi_write_png(filename.data(), w, h, channels, image_data.data(), w * channels);
    Timer timer;
    if(!glfwInit())
        return -1;
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(w, h, windowTitle.data(), nullptr, nullptr);
    if(!window) {
        LERROR("Failed to create GLFW window");
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
    glfwSwapInterval(1);
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

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("./src/default.vert", "./src/default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices.data(), sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices.data(), sizeof(indices));

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_DOUBLE, 8 * doublesize, nullptr);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_DOUBLE, 8 * doublesize, (void *)(3 * doublesize));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_DOUBLE, 8 * doublesize, (void *)(6 * doublesize));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    timer3.stop();
    const double shadersetup = timer3.elapsedSeconds();
    LINFO("shaders,vao evbo setup eseguito in {:f}", shadersetup);
    const double totalT = totalw + shadersetup;
    LINFO("total setup time {:f}", totalT);

    // Gets ID of uniform called "scale"
    const GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture popCat(filename.data(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);

    // Main while loop
    while(!glfwWindowShouldClose(window)) {
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
        glUniform1f(uniID, 0.0f);
        // Binds texture so that is appears in rendering
        popCat.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    popCat.Delete();
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
