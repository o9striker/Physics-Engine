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
    for (auto& p : particles) {
        if (p.isStatic) continue;

        float halfW = (p.shape == BOX) ? p.width / 2.0f : p.radius;
        float halfH = (p.shape == BOX) ? p.height / 2.0f : p.radius;

        // Floor
        if (p.position.y + halfH >= floorY) {
            p.position.y = floorY - halfH;
            if (p.velocity.y < 30.0f) {
                p.velocity.y = 0.0f; // Put to sleep vertically
            } else {
                p.velocity.y *= -0.8f; // Bounce with restitution
            }
            // Apply horizontal friction when rubbing against the floor
            p.velocity.x *= 0.95f; 
        }

        // Left wall
        if (p.position.x - halfW <= wallLeft) {
            p.position.x = wallLeft + halfW;
            p.velocity.x *= -0.8f;
            // Apply vertical friction when rubbing against the wall
            p.velocity.y *= 0.95f;
        }

        // Right wall
        if (p.position.x + halfW >= wallRight) {
            p.position.x = wallRight - halfW;
            p.velocity.x *= -0.8f;
            // Apply vertical friction when rubbing against the wall
            p.velocity.y *= 0.95f;
        }
    }

    // 4. Resolve Particle-Particle Collisions
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            particles[i].ResolveCollision(particles[j]);
        }
    }
}
