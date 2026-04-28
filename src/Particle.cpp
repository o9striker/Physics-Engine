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

void Particle::Update(float deltaTime, int screenWidth, int screenHeight) {
    // 1. Add gravity to the velocity's Y-component
    velocity.y += GRAVITY * deltaTime;

    // 2. Update the particle's position by adding the current velocity
    position += velocity * deltaTime;

    // Floor Collision
    if (position.y + radius >= screenHeight) {
        position.y = (float)screenHeight - radius;
        velocity.y *= -0.8f; // Bounce with restitution
    }

    // Left/Right Wall Collision
    if (position.x - radius <= 0) {
        position.x = radius;
        velocity.x *= -0.8f;
    } else if (position.x + radius >= screenWidth) {
        position.x = (float)screenWidth - radius;
        velocity.x *= -0.8f;
    }
}

void Particle::ResolveCollision(Particle& other) {
    // Step 1: Detection (The Pythagorean Check)
    float dist = glm::distance(position, other.position);
    float sumRadii = radius + other.radius;

    if (dist >= sumRadii) {
        return; // No collision
    }

    // Safeguard: if they are exactly at the same position, shift one slightly
    if (dist == 0.0f) {
        position.x += 0.01f;
        dist = glm::distance(position, other.position);
    }

    // Step 2: Positional Correction (Anti-Squish)
    float overlap = sumRadii - dist;
    glm::vec2 normal = glm::normalize(position - other.position);

    // Push apart by half the overlap
    position += normal * (overlap * 0.5f);
    other.position -= normal * (overlap * 0.5f);

    // Step 3: Velocity Resolution (The Magic Bounce)
    glm::vec2 v_rel = velocity - other.velocity;
    float velAlongNormal = glm::dot(v_rel, normal);

    // Sanity Check: If they are moving apart, do nothing
    if (velAlongNormal > 0) {
        return;
    }

    // Calculate Impulse (j)
    float e = 0.8f; // Restitution
    float j = -(1.0f + e) * velAlongNormal;
    j /= (1.0f / mass) + (1.0f / other.mass);

    // Apply impulse to velocities
    glm::vec2 impulse = j * normal;
    velocity += impulse / mass;
    other.velocity -= impulse / other.mass;
}
