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

# ---- Sources ----
file(GLOB_RECURSE SCENE_GRAPH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/sceneGraph/*"
)

file(GLOB_RECURSE RENDER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/render/*"
)

file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*"
)

file(GLOB_RECURSE HEADER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*"
)

# ---- Executable ----
add_executable(${PROJECT_NAME}
    main.cpp
    ${SRC_SOURCES}
    ${HEADER_SOURCES}
    ${SCENE_GRAPH_SOURCES}
    ${RENDER_SOURCES}
)

# ---- VS groups ----
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/render PREFIX "Render" FILES ${RENDER_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/sceneGraph PREFIX "SceneGraph" FILES ${SCENE_GRAPH_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Src" FILES ${SRC_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/headers PREFIX "Headers" FILES ${HEADER_SOURCES})

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
        ${CMAKE_CURRENT_SOURCE_DIR}/sceneGraph
        ${CMAKE_CURRENT_SOURCE_DIR}/sceneGraph/headers
        ${CMAKE_CURRENT_SOURCE_DIR}/sceneGraph/src
        ${CMAKE_CURRENT_SOURCE_DIR}/render
        ${CMAKE_CURRENT_SOURCE_DIR}/render/headers
        ${CMAKE_CURRENT_SOURCE_DIR}/render/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)