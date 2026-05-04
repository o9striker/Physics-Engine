#pragma once
#include <vector>
#include "Particle.h"
#include "Spring.h"
#include "Pulley.h"

struct EngineState {
    bool enableGravity = true;
    bool enableCollisions = true;
    bool showSprings = true;
    float gravityValue = 981.0f;

    int spawnShape = 0; // 0 = Circle, 1 = Box
    bool sceneMenuOpen = false;
    int currentScene = 1; // Default to Atwood Machine
};

class PhysicsWorld {
public:
    std::vector<Particle> particles;
    std::vector<Spring> springs;
    std::vector<Pulley> pulleys;

    // Environmental Constraints
    float floorY;
    float wallLeft;
    float wallRight;

    // Constructor to set up boundaries
    PhysicsWorld(float fY, float wL, float wR);

    // Run a single simulation step
    void Update(float dt, const EngineState& state);
};
