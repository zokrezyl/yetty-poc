pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

// Redirect Gradle build to ANDROID_BUILD_DIR (go up two levels: build-tools/android -> root)
rootProject.projectDir.parentFile.parentFile.let { projectRoot ->
    val buildDir = System.getenv("ANDROID_BUILD_DIR") ?: "${projectRoot}/build-android"
    gradle.projectsLoaded {
        allprojects {
            layout.buildDirectory.set(File(buildDir, project.name))
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
