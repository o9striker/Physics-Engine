#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(float fY, float wL, float wR) {
    floorY = fY;
    wallLeft = wL;
    wallRight = wR;
}

void PhysicsWorld::Update(float dt) {
    // 1. Apply Spring Forces
    for (auto& s : springs) {
        s.Update(particles);
    }

    // 2. Integrate (advance positions via forces + gravity)
    for (auto& p : particles) {
        p.Update(dt);
    }

    // 3. Enforce Boundary Constraints
    //    The Particle is now blissfully unaware of walls and floors.
    for (auto& p : particles) {
        if (p.isStatic) continue;

        // Floor
        if (p.position.y + p.radius >= floorY) {
            p.position.y = floorY - p.radius;
            if (p.velocity.y < 30.0f) {
                p.velocity.y = 0.0f; // Put to sleep vertically
            } else {
                p.velocity.y *= -0.8f; // Bounce with restitution
            }
        }

        // Left wall
        if (p.position.x - p.radius <= wallLeft) {
            p.position.x = wallLeft + p.radius;
            p.velocity.x *= -0.8f;
        }

        // Right wall
        if (p.position.x + p.radius >= wallRight) {
            p.position.x = wallRight - p.radius;
            p.velocity.x *= -0.8f;
        }
    }

    // 4. Resolve Particle-Particle Collisions
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            particles[i].ResolveCollision(particles[j]);
        }
    }
}
