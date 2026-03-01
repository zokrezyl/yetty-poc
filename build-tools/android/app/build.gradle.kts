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
            abiFilters += listOf("arm64-v8a")  // Primary target
        }

        externalNativeBuild {
            cmake {
                val webgpuBackend = System.getenv("WEBGPU_BACKEND") ?: "wgpu"
                val buildDir = System.getenv("ANDROID_BUILD_DIR") ?: "${rootProject.projectDir.parentFile.parentFile}/build-android"
                arguments += listOf(
                    "-DYETTY_ANDROID=ON",
                    "-DCMAKE_BUILD_TYPE=Release",
                    "-DWEBGPU_BACKEND=${webgpuBackend}",
                    "-DANDROID_BUILD_DIR=${buildDir}"
                )
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
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

    // Include pre-built libraries and assets from ANDROID_BUILD_DIR
    val buildDir = System.getenv("ANDROID_BUILD_DIR") ?: "${rootProject.projectDir.parentFile.parentFile}/build-android"
    val webgpuBackend = System.getenv("WEBGPU_BACKEND") ?: "wgpu"
    val libsDir = if (webgpuBackend == "dawn") "dawn-libs" else "wgpu-libs"
    sourceSets {
        getByName("main") {
            jniLibs.srcDirs(File(buildDir, libsDir))
            assets.srcDirs(File(buildDir, "assets"))
        }
    }
}

dependencies {
    // No Java dependencies needed - pure native app
}

// APK is already in the correct directory (ANDROID_BUILD_DIR/app/outputs/apk/)
// No additional copy needed since layout.buildDirectory is set to ANDROID_BUILD_DIR
