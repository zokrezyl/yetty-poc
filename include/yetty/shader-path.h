#pragma once
// Runtime shader directory resolution for deployed builds.
// Checks: 1) YETTY_SHADERS_DIR env var  2) assets/shaders next to exe  3) compile-time fallback

#include <filesystem>
#include <cstdlib>
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <climits>
#elif defined(__linux__)
#include <unistd.h>
#include <climits>
#endif

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

#ifndef YETTY_SHADERS_DIR
#define YETTY_SHADERS_DIR CMAKE_SOURCE_DIR "/src/yetty/shaders"
#endif

namespace yetty {

inline std::string getShadersDir() {
    // 1) Environment variable override
    const char* envDir = std::getenv("YETTY_SHADERS_DIR");
    if (envDir && envDir[0]) {
        return std::string(envDir);
    }

#if !defined(YETTY_WEB) || !YETTY_WEB
    // 2) Check for assets/shaders relative to executable (deployed builds)
    std::filesystem::path exeDir;
    #if defined(_WIN32)
    {
        char path[MAX_PATH];
        if (GetModuleFileNameA(nullptr, path, MAX_PATH) > 0) {
            exeDir = std::filesystem::path(path).parent_path();
        }
    }
    #elif defined(__linux__)
    {
        char path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (len != -1) { path[len] = '\0'; exeDir = std::filesystem::path(path).parent_path(); }
    }
    #elif defined(__APPLE__)
    {
        char path[PATH_MAX];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0) {
            char realPath[PATH_MAX];
            if (realpath(path, realPath)) { exeDir = std::filesystem::path(realPath).parent_path(); }
        }
    }
    #endif
    if (!exeDir.empty()) {
        auto deployedShaders = exeDir / "assets" / "shaders";
        if (std::filesystem::exists(deployedShaders)) {
            return deployedShaders.string();
        }
    }
#endif

    // 3) Compile-time fallback (source tree path)
    return std::string(YETTY_SHADERS_DIR);
}

inline std::string getAssetsDir() {
    const char* envDir = std::getenv("YETTY_ASSETS_DIR");
    if (envDir && envDir[0]) {
        return std::string(envDir);
    }

#if !defined(YETTY_WEB) || !YETTY_WEB
    std::filesystem::path exeDir;
    #if defined(_WIN32)
    {
        char path[MAX_PATH];
        if (GetModuleFileNameA(nullptr, path, MAX_PATH) > 0) {
            exeDir = std::filesystem::path(path).parent_path();
        }
    }
    #elif defined(__linux__)
    {
        char path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (len != -1) { path[len] = '\0'; exeDir = std::filesystem::path(path).parent_path(); }
    }
    #elif defined(__APPLE__)
    {
        char path[PATH_MAX];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0) {
            char realPath[PATH_MAX];
            if (realpath(path, realPath)) { exeDir = std::filesystem::path(realPath).parent_path(); }
        }
    }
    #endif
    if (!exeDir.empty()) {
        auto deployedAssets = exeDir / "assets";
        if (std::filesystem::exists(deployedAssets)) {
            return deployedAssets.string();
        }
    }
#endif

#ifdef YETTY_ASSETS_DIR
    return std::string(YETTY_ASSETS_DIR);
#else
    return std::string(CMAKE_SOURCE_DIR) + "/assets";
#endif
}

} // namespace yetty
