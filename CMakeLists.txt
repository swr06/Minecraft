cmake_minimum_required(VERSION 3.11)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.265.tar.gz"
    SHA1 "eae0bb5b7b8aa968e0ff87ec51be51b4255db110"
    LOCAL
)

project(minecraft)

hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)
hunter_add_package(glfw)
find_package(glfw3 REQUIRED)
hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)
hunter_add_package(imgui)
find_package(imgui CONFIG REQUIRED)

#allows setting working folder in debug/release builds
hunter_add_package(CreateLaunchers)
find_package(CreateLaunchers CONFIG REQUIRED)

include(CreateLaunchers)

add_subdirectory(Source)

file(COPY ${CMAKE_SOURCE_DIR}/Source/Shaders ${CMAKE_SOURCE_DIR}/Source/Resources DESTINATION ${CMAKE_BINARY_DIR}/Core/)
