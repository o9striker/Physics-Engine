#include "Particle.h"

const float GRAVITY = 981.0f; // Pixels per second squared

Particle::Particle(float x, float y, float m) {
    position = glm::vec2(x, y);
    velocity = glm::vec2(0.0f, 0.0f);
    acceleration = glm::vec2(0.0f, 0.0f);
    mass = m;
}

void Particle::Update(float deltaTime) {
    // 1. Add gravity to the velocity's Y-component
    velocity.y += GRAVITY * deltaTime;

    // 2. Update the particle's position by adding the current velocity
    position += velocity * deltaTime;
}
