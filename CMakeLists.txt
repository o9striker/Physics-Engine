cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Find Raylib
find_package(raylib REQUIRED)

# Include directories
include_directories(include)

# Add your main file
add_executable(PhysicsEngine src/main.cpp)

# Link Raylib to your project
target_link_libraries(PhysicsEngine PRIVATE raylib)
