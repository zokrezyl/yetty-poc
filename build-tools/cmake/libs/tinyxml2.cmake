# tinyxml2 - XML parser
if(TARGET tinyxml2::tinyxml2)
    return()
endif()

set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
CPMAddPackage(
    NAME tinyxml2
    GITHUB_REPOSITORY leethomason/tinyxml2
    GIT_TAG 10.0.0
)
