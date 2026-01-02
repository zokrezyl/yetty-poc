pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

// Redirect Gradle cache to build-android (go up two levels: build-tools/android -> root)
rootProject.projectDir.parentFile.parentFile.let { projectRoot ->
    gradle.projectsLoaded {
        allprojects {
            layout.buildDirectory.set(File(projectRoot, "build-android/${project.name}"))
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
