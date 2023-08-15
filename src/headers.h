#pragma once
// #define GLFW_INCLUDE_VULKAN NOSONAR
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_WIN32
#define GLM_FORCE_SIZE_T_LENGTH
#define GLM_FORCE_PURE
#define GLM_FORCE_INLINE
#define GLM_FORCE_EXPLICIT_CTOR
#define GLM_FORCE_CXX20
#define GLM_FORCE_UNRESTRICTED_GENTYPE
#define GLM_FORCE_PRECISION_HIGHP_DOUBLE
#define GLM_FORCE_PRECISION_HIGHP_INT
#define GLM_FORCE_PRECISION_HIGHP_FLOAT
#define VMA_IMPLEMENTATION
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(push)
#pragma warning(disable : 6386 6385 4005)
#endif
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <execution>
#include <format>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <numbers>
#include <omp.h>
#include <optional>
#include <random>
#include <ranges>
#include <set>
#include <spdlog/cfg/env.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string_view>
#include <unordered_set>
#include <vector>
// clang-format on

#include "casts.h"
// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(pop)
#endif

[[nodiscard]] static constexpr auto calcolaCentro(const unsigned int width, const unsigned int w) noexcept {
    return (width - w) / 2;
}

#define LTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LWARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LCRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define CALC_CENTRO(width, w) calcolaCentro(width, w)
#define GLWFERR(error, description) LERROR("GLFW Error ({}): {}", error, description);
#define SYSPAUSE()                                                                                                               \
    LINFO("Press enter to exit...");                                                                                             \
    std::cin.ignore();

static inline constexpr long double infinity = std::numeric_limits<long double>::infinity();
static inline constexpr long double pi = std::numbers::pi_v<long double>;
static inline constexpr long double twoPi = 2 * pi;
static inline constexpr long double halfPi = pi / 2;
// Dimensioni della finestra
static inline constexpr double ratioW = 16.0;
static inline constexpr double ratioH = 9.0;
static inline constexpr double aspect_ratio = ratioW / ratioH;
static inline constexpr int imageF = 80;
static inline constexpr int w = C_I(ratioW * imageF);
static inline constexpr int h = C_I(ratioH * imageF);
static inline constexpr std::size_t ST_w = C_ST(w);
static inline constexpr std::size_t ST_h = C_ST(h);
static inline constexpr double scale = 256.0;
static inline constexpr double invStHMinusOne = 1.0 / C_D(ST_h - 1);
static inline constexpr double invStWMinusOne = 1.0 / C_D(ST_w - 1);
static inline constexpr std::string_view filename = "./src/output_image.png";
static inline constexpr std::string_view windowTitle = "Ray Tracer";
static inline constexpr unsigned long long doublesize = sizeof(double);

[[nodiscard]] static inline double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }

[[nodiscard]] static inline double random_double() {
    static std::uniform_real_distribution distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

[[nodiscard]] static inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}
