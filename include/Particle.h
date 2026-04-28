#pragma once
#include <glm/glm.hpp>
#include <cstdint>

class Particle {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    glm::vec2 forceAccumulator; // To gather forces during the frame
    float mass;
    float radius;
    uint32_t color; // Hex color: RRGGBBAA

    // Constructor to set up a new particle
    Particle(float x, float y, float m, float r, uint32_t c);

    // Apply an external force to this particle
    void ApplyForce(glm::vec2 force);

    // The function that will move the particle and handle bounds collision
    void Update(float deltaTime, int screenWidth, int screenHeight);

    // Resolves a physics collision with another particle
    void ResolveCollision(Particle& other);
};
