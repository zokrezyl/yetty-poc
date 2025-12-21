#include "renderer/WebGPUContext.h"
#include "renderer/TextRenderer.h"
#include "terminal/Grid.h"
#include "terminal/Font.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace yetty;

// Default font path - adjust as needed for your system
#if defined(_WIN32)
const char* DEFAULT_FONT = "C:/Windows/Fonts/consola.ttf";
#elif defined(__APPLE__)
const char* DEFAULT_FONT = "/System/Library/Fonts/Monaco.ttf";
#else
const char* DEFAULT_FONT = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
#endif

int main(int argc, char* argv[]) {
    const char* fontPath = (argc > 1) ? argv[1] : DEFAULT_FONT;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    // Don't create OpenGL context - we're using WebGPU
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    uint32_t width = 1024;
    uint32_t height = 768;

    GLFWwindow* window = glfwCreateWindow(width, height, "yetty - WebGPU Terminal", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Initialize WebGPU
    WebGPUContext ctx;
    if (!ctx.init(window, width, height)) {
        std::cerr << "Failed to initialize WebGPU" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Generate MSDF font atlas
    Font font;
    float fontSize = 32.0f;
    if (!font.generate(fontPath, fontSize)) {
        std::cerr << "Failed to generate font atlas from: " << fontPath << std::endl;
        std::cerr << "Usage: " << argv[0] << " [path-to-ttf-font]" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    if (!font.createTexture(ctx.getDevice(), ctx.getQueue())) {
        std::cerr << "Failed to create font texture" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Initialize text renderer
    TextRenderer renderer;
    float cellWidth = fontSize * 0.6f;   // Approximate monospace width
    float cellHeight = fontSize * 1.2f;  // Line height
    renderer.setCellSize(cellWidth, cellHeight);
    renderer.resize(width, height);

    if (!renderer.init(ctx, font)) {
        std::cerr << "Failed to initialize text renderer" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Calculate grid size based on window
    uint32_t cols = static_cast<uint32_t>(width / cellWidth);
    uint32_t rows = static_cast<uint32_t>(height / cellHeight);
    Grid grid(cols, rows);

    // Demo content
    const char* lines[] = {
        "Welcome to yetty - WebGPU Terminal Emulator",
        "============================================",
        "",
        "This is a proof-of-concept terminal renderer using:",
        "  * WebGPU (via wgpu-native)",
        "  * MSDF (Multi-channel Signed Distance Field) fonts",
        "  * GPU-accelerated text rendering",
        "",
        "Features:",
        "  - Crisp text at any scale",
        "  - Colored text and backgrounds",
        "  - Fast GPU-based rendering",
        "",
        "Press ESC to exit.",
    };

    // Colors
    glm::vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 green = {0.0f, 1.0f, 0.0f, 1.0f};
    glm::vec4 cyan = {0.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 yellow = {1.0f, 1.0f, 0.0f, 1.0f};
    glm::vec4 magenta = {1.0f, 0.0f, 1.0f, 1.0f};

    // Debug: print what we're writing
    std::cout << "Writing demo content to grid (" << cols << "x" << rows << "):" << std::endl;
    for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]); ++i) {
        std::cout << "  Line " << i << ": \"" << lines[i] << "\"" << std::endl;
    }

    // Write demo content
    for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]) && i < rows; ++i) {
        glm::vec4 color = white;
        if (i == 0) color = green;
        else if (i == 1) color = green;
        else if (lines[i][0] == ' ' && lines[i][1] == ' ' && lines[i][2] == '*') color = cyan;
        else if (lines[i][0] == ' ' && lines[i][1] == ' ' && lines[i][2] == '-') color = yellow;

        grid.writeString(0, static_cast<uint32_t>(i), lines[i], color);
    }

    // Window resize callback
    glfwSetWindowUserPointer(window, &ctx);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int newWidth, int newHeight) {
        if (newWidth == 0 || newHeight == 0) return;
        auto* context = static_cast<WebGPUContext*>(glfwGetWindowUserPointer(w));
        context->resize(static_cast<uint32_t>(newWidth), static_cast<uint32_t>(newHeight));
    });

    std::cout << "Starting render loop..." << std::endl;

    // Main loop
    double lastTime = glfwGetTime();
    uint32_t frameCount = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Check for ESC key
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // Get current window size
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        if (w > 0 && h > 0) {
            renderer.resize(static_cast<uint32_t>(w), static_cast<uint32_t>(h));
        }

        // Render
        renderer.render(ctx, grid);

        // FPS counter
        frameCount++;
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 1.0) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    std::cout << "Shutting down..." << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
