pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

// Redirect Gradle build to build-android-{backend} (go up two levels: build-tools/android -> root)
rootProject.projectDir.parentFile.parentFile.let { projectRoot ->
    val webgpuBackend = System.getenv("WEBGPU_BACKEND") ?: "wgpu"
    gradle.projectsLoaded {
        allprojects {
            layout.buildDirectory.set(File(projectRoot, "build-android-${webgpuBackend}/${project.name}"))
        }
    }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "yetty"
include(":app")
