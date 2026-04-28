cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Find Raylib
find_package(raylib REQUIRED)

# Find GLM
find_package(glm REQUIRED)

# Include directories
include_directories(include)

# Add your main file
add_executable(PhysicsEngine src/main.cpp src/Particle.cpp src/Spring.cpp)

# Link libraries to your project
target_link_libraries(PhysicsEngine PRIVATE raylib glm::glm)
