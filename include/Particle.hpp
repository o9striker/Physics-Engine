#pragma once
#include <glm/glm.hpp>
#include <cstdint>

enum ShapeType { CIRCLE, BOX };

class Particle {
public:
    ShapeType shape;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    glm::vec2 forceAccumulator; // To gather forces during the frame
    float mass;
    float radius;
    float width;
    float height;
    uint32_t color; // Hex color: RRGGBBAA
    bool isStatic = false; // If true, the particle will not be updated by physics

    // Constructor to set up a new particle (Circle)
    Particle(float x, float y, float m, float r, uint32_t c);

    // Constructor to set up a new particle (Box)
    Particle(float x, float y, float m, float w, float h, uint32_t c);

    // Apply an external force to this particle
    void ApplyForce(glm::vec2 force);

    // Integrates forces and advances position. Boundary enforcement is handled by PhysicsWorld.
    void Update(float deltaTime);

    // Resolves a physics collision with another particle
    void ResolveCollision(Particle& other);
};
