#pragma once
#include <vector>
#include "Particle.h"
#include "Spring.h"
#include "Pulley.h"

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

    // Run a single physics tick
    void Update(float dt);
};
