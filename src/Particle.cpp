#include "Particle.h"

const float GRAVITY = 981.0f; // Pixels per second squared

Particle::Particle(float x, float y, float m, float r, uint32_t c) {
    position = glm::vec2(x, y);
    velocity = glm::vec2(0.0f, 0.0f);
    acceleration = glm::vec2(0.0f, 0.0f);
    mass = m;
    radius = r;
    color = c;
}

void Particle::Update(float deltaTime, int screenHeight) {
    // 1. Add gravity to the velocity's Y-component
    velocity.y += GRAVITY * deltaTime;

    // 2. Update the particle's position by adding the current velocity
    position += velocity * deltaTime;

    // Floor Collision
    if (position.y + radius >= screenHeight) {
        position.y = (float)screenHeight - radius;
        velocity.y *= -0.8f; // Bounce with restitution
    }
}
