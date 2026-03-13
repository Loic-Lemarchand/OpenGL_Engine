cmake_minimum_required(VERSION 3.21)

project(OPENGL_Engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Dependencies ----
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# ---- Executable ----
add_executable(${PROJECT_NAME}
    main.cpp
    window.cpp
    renderer.cpp
    shader.cpp

    utilities.cpp
    inputManager.cpp
    eventBus.cpp
    camera.cpp

    window.h
    renderer.h
    shader.h

    utilities.h
    inputManager.h
    eventBus.h
    camera.h
)

# ---- Linking ----
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        glfw
        glad::glad
        glm::glm
)

# ---- Include dirs ----
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)