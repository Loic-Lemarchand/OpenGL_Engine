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
find_package(assimp CONFIG REQUIRED)

# ---- Executable ----
add_executable(${PROJECT_NAME}
    main.cpp
    window.cpp
    renderer.cpp
    shader.cpp
    texture.cpp
    mesh.cpp
    model.cpp

    utilities.cpp
    inputManager.cpp
    eventBus.cpp
    camera.cpp
    light.cpp

    window.h
    renderer.h
    shader.h
    texture.h
    mesh.h
    model.h

    utilities.h
    inputManager.h
    eventBus.h
    camera.h
    light.h

    stb_image.h

    Wood_Color.jpg
)

# ---- Linking ----
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        glfw
        glad::glad
        glm::glm
        assimp::assimp
)

# ---- Include dirs ----
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)