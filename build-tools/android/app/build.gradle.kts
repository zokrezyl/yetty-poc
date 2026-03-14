plugins {
    id("com.android.application")
}

android {
    namespace = "com.yetty.terminal"
    compileSdk = 34
    ndkVersion = "26.1.10909125"

    defaultConfig {
        applicationId = "com.yetty.terminal"
        minSdk = 28
        targetSdk = 34
        versionCode = 1
        versionName = "0.1.0"

        ndk {
            // Architecture from env var: arm64-v8a (default) or x86_64
            val androidAbi = System.getenv("ANDROID_ABI") ?: "arm64-v8a"
            abiFilters += listOf(androidAbi)
        }

        externalNativeBuild {
            cmake {
                val webgpuBackend = System.getenv("WEBGPU_BACKEND") ?: "dawn"
                val buildDir = System.getenv("ANDROID_BUILD_DIR") ?: "${rootProject.projectDir.parentFile.parentFile}/build-android"
                val useCcache = System.getenv("USE_CCACHE") == "1"
                val cmakeArgs = mutableListOf(
                    "-DYETTY_ANDROID=ON",
                    "-DCMAKE_BUILD_TYPE=Release",
                    "-DWEBGPU_BACKEND=${webgpuBackend}",
                    "-DANDROID_BUILD_DIR=${buildDir}",
                    "-DANDROID_STL=c++_static"
                )
                if (useCcache) {
                    cmakeArgs += listOf(
                        "-DCMAKE_C_COMPILER_LAUNCHER=ccache",
                        "-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
                    )
                }
                arguments += cmakeArgs
            }
        }
    }

    signingConfigs {
        getByName("debug") {
            // Uses default debug keystore - allows installing release APK for testing
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            signingConfig = signingConfigs.getByName("debug")
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
        debug {
            isDebuggable = true
            externalNativeBuild {
                cmake {
                    arguments += "-DCMAKE_BUILD_TYPE=Debug"
                    // WEBGPU_BACKEND and ANDROID_BUILD_DIR already set in defaultConfig
                }
            }
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
            // Redirect CMake build output to ANDROID_BUILD_DIR/cxx
            val buildDir = System.getenv("ANDROID_BUILD_DIR") ?: "${rootProject.projectDir.parentFile.parentFile}/build-android"
            buildStagingDirectory = File(buildDir, "cxx")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }

    buildFeatures {
        prefab = true
    }

    packaging {
        jniLibs {
            useLegacyPackaging = true  // Extract native libs to lib/ directory
        }
    }

    // Include pre-built libraries and assets from ANDROID_BUILD_DIR and source directories
    val buildDir = System.getenv("ANDROID_BUILD_DIR") ?: "${rootProject.projectDir.parentFile.parentFile}/build-android"
    val projectRoot = rootProject.projectDir.parentFile.parentFile
    val webgpuBackend = System.getenv("WEBGPU_BACKEND") ?: "dawn"
    val libsDir = "dawn-libs"  // Only Dawn is supported
    sourceSets {
        getByName("main") {
            jniLibs.srcDirs(File(buildDir, libsDir), File(buildDir, "jniLibs"))
            assets.srcDirs(
                File(buildDir, "assets"),
                File(projectRoot, "demo")  // Include demo directory
            )
        }
    }
}

dependencies {
    // No Java dependencies needed - pure native app
}

// APK is already in the correct directory (ANDROID_BUILD_DIR/app/outputs/apk/)
// No additional copy needed since layout.buildDirectory is set to ANDROID_BUILD_DIR
