cmake_minimum_required(VERSION 3.14)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Fetch Raylib from source to avoid the "White Screen" bug in pre-built binaries
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
)

# Fetch GLM from source (header only)
FetchContent_Declare(
  glm
  URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
)

FetchContent_MakeAvailable(raylib glm)

# Include directories
include_directories(include)

# Add your main file
add_executable(PhysicsEngine src/main.cpp src/Particle.cpp src/Spring.cpp src/PhysicsWorld.cpp)

# Link libraries to your project
target_link_libraries(PhysicsEngine PRIVATE raylib glm::glm)
