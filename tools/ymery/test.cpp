// Ymery test program - loads a YAML layout from ymery-cpp demo and renders it
// Usage: ymery-test <path>
//   path: either a .yaml file (e.g. ../ymery-cpp/demo/layouts/simple/app.yaml)
//         or a layout directory (e.g. ../ymery-cpp/demo/layouts/simple)

#include "../../src/yetty/ymery/lang.h"
#include "../../src/yetty/ymery/renderer.h"
#include "../../src/yetty/ymery/dispatcher.h"
#include "../../src/yetty/ymery/simple-data-tree.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::cfg::load_env_levels();

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        fprintf(stderr, "  path: a .yaml file or a layout directory\n");
        fprintf(stderr, "  e.g.: %s ../ymery-cpp/demo/layouts/simple/app.yaml\n", argv[0]);
        fprintf(stderr, "  e.g.: %s ../ymery-cpp/demo/layouts/imgui\n", argv[0]);
        return 1;
    }

    fs::path inputPath = fs::absolute(argv[1]);
    if (!fs::exists(inputPath)) {
        fprintf(stderr, "Path not found: %s\n", inputPath.c_str());
        return 1;
    }

    // Determine layout search dirs and main module
    // If input is a file like .../layouts/simple/app.yaml:
    //   - module dir = .../layouts/simple (search here first)
    //   - parent dir = .../layouts (search here for shared.* imports)
    //   - main module = "app" (stem of the yaml file)
    // If input is a directory like .../layouts/simple:
    //   - module dir = .../layouts/simple
    //   - parent dir = .../layouts
    //   - main module = "app"
    std::string mainModule = "app";
    std::vector<fs::path> searchPaths;

    if (fs::is_regular_file(inputPath)) {
        mainModule = inputPath.stem().string(); // "app" from "app.yaml"
        fs::path moduleDir = inputPath.parent_path();
        fs::path parentDir = moduleDir.parent_path();
        searchPaths = {moduleDir, parentDir};
    } else {
        fs::path parentDir = inputPath.parent_path();
        searchPaths = {inputPath, parentDir};
    }

    printf("Search paths:\n");
    for (const auto& p : searchPaths) {
        printf("  %s\n", p.c_str());
    }
    printf("Main module: %s\n", mainModule.c_str());

    // Load YAML
    auto langRes = yetty::ymery::Lang::create(searchPaths, mainModule);
    if (!langRes) {
        fprintf(stderr, "Failed to load YAML: %s\n", langRes.error().to_string().c_str());
        return 1;
    }
    auto lang = *langRes;

    printf("Loaded %zu widget definitions\n", lang->widgetDefinitions().size());
    for (const auto& [name, _] : lang->widgetDefinitions()) {
        printf("  widget: %s\n", name.c_str());
    }

    // Create dispatcher
    auto dispRes = yetty::ymery::Dispatcher::create();
    if (!dispRes) {
        fprintf(stderr, "Failed to create dispatcher\n");
        return 1;
    }
    auto dispatcher = *dispRes;

    // Create simple data tree
    auto treeRes = yetty::ymery::SimpleDataTree::create();
    if (!treeRes) {
        fprintf(stderr, "Failed to create data tree\n");
        return 1;
    }
    auto dataTree = *treeRes;

    // Create renderer
    auto rendererRes = yetty::ymery::Renderer::create(lang, dispatcher, dataTree);
    if (!rendererRes) {
        fprintf(stderr, "Failed to create renderer: %s\n", rendererRes.error().to_string().c_str());
        return 1;
    }
    auto renderer = *rendererRes;

    printf("Render list:\n");
    // The renderer picks up root-widget from app config automatically

    // ---- GLFW + OpenGL + ImGui setup ----
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        return 1;
    }

    // GL 3.3 + GLSL 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Ymery Test", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    printf("Running... close window to exit.\n");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ymery widgets
        if (auto res = renderer->renderFrame(); !res) {
            spdlog::error("renderFrame failed: {}", res.error().to_string());
        }

        // Rendering
        ImGui::Render();
        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
