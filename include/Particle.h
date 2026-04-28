#pragma once
#include <glm/glm.hpp>
#include <cstdint>

class Particle {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass;
    float radius;
    uint32_t color; // Hex color: RRGGBBAA

    // Constructor to set up a new particle
    Particle(float x, float y, float m, float r, uint32_t c);

    // The function that will move the particle and handle bounds collision
    void Update(float deltaTime, int screenWidth, int screenHeight);

    // Resolves a physics collision with another particle
    void ResolveCollision(Particle& other);
};
