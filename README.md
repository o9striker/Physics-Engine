# 2D Physics Engine

![Physics Engine Preview](https://raw.githubusercontent.com/o9striker/Physics-Engine/main/docs/preview.png)

A custom-built, feature-rich 2D physics simulation environment developed in C++ using **Raylib** for rendering and **GLM** for vector mathematics. This engine supports complex mechanical constraints, multi-shape collision detection, and a real-time interactive UI.

## 🚀 Key Features

- **Multi-Shape Collision Detection**: Support for Circle-Circle, Box-Box (AABB), and Circle-Box collisions with robust resolution.
- **Advanced Constraints**:
  - **Springs**: Hooke's Law integration with damping for elastic connections.
  - **Pulleys**: Simulation of tension and length-constrained systems (e.g., Atwood Machine).
- **Physical Accuracy**:
  - Fixed time-step integration for deterministic simulations.
  - Weighted positional correction to prevent overlap jitter.
  - Tangent-based friction and axis of least penetration for realistic box physics.
- **Interactive Sandbox**:
  - Drag-and-drop particles to manipulate the simulation in real-time.
  - Dynamic spawning of shapes (Circles/Boxes) via mouse clicks.
- **Real-time UI**: Built-in control panel using **Raygui** to toggle gravity, collisions, and switch between predefined scenes.

## 🛠 Tech Stack

- **Language**: C++17
- **Rendering**: [Raylib 5.5](https://www.raylib.com/)
- **Math**: [GLM (OpenGL Mathematics)](https://github.com/g-truc/glm)
- **Build System**: CMake

## 🎮 Interactive Demos

The engine includes three curated scenes to showcase its capabilities:

1.  **Sandbox**: A free-form environment for testing individual constraints and spawning shapes.
2.  **Atwood Machine**: A classic physics demonstration featuring a pulley system with differential weights.
3.  **Block Tower**: A stability test for the collision engine, featuring stacked boxes under gravitational load.

## 🏗 Build Instructions

### Prerequisites
- CMake (3.14+)
- A C++17 compatible compiler (GCC/Clang/MSVC)

### Compiling
```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the engine
./PhysicsEngine
```

## ⌨️ Controls

- **Left Click (Empty Space)**: Spawn a new shape (Circle/Box).
- **Left Click (On Shape)**: Grab and drag a particle.
- **UI Panel**: Toggle simulation parameters like Gravity and Collision resolution.

## 📂 Project Structure

- `include/`: Header files for physics logic and constraints.
- `src/`: Implementation files and the main simulation loop.
- `CMakeLists.txt`: Build configuration fetching dependencies automatically from source.

---
Developed by **Alphacew**
