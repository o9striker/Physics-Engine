#include "Particle.h"

const float GRAVITY = 981.0f; // Pixels per second squared

Particle::Particle(float x, float y, float m, float r, uint32_t c) {
    position = glm::vec2(x, y);
    velocity = glm::vec2(0.0f, 0.0f);
    acceleration = glm::vec2(0.0f, 0.0f);
    forceAccumulator = glm::vec2(0.0f, 0.0f);
    mass = m;
    radius = r;
    color = c;
    isStatic = false;
}

void Particle::ApplyForce(glm::vec2 force) {
    if (isStatic) return;
    forceAccumulator += force;
}

void Particle::Update(float deltaTime, int screenWidth, int screenHeight) {
    if (isStatic) {
        forceAccumulator = glm::vec2(0.0f, 0.0f);
        return;
    }

    // 1. Calculate acceleration from accumulated forces (Newton's 2nd Law: a = F/m)
    acceleration = forceAccumulator / mass;

    // 2. Add gravity to the acceleration
    acceleration.y += GRAVITY;

    // 3. Update velocity using the total acceleration
    velocity += acceleration * deltaTime;

    // 4. Update the position by adding the current velocity
    position += velocity * deltaTime;

    // 5. Clear forces for the next frame
    forceAccumulator = glm::vec2(0.0f, 0.0f);

    // Floor Collision
    if (position.y + radius >= screenHeight) {
        position.y = (float)screenHeight - radius;
        if (velocity.y < 30.0f) {
            velocity.y = 0.0f; // Put to sleep vertically
        } else {
            velocity.y *= -0.8f; // Bounce with restitution
        }
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

    // Push apart based on static flags
    if (!isStatic && !other.isStatic) {
        position += normal * (overlap * 0.5f);
        other.position -= normal * (overlap * 0.5f);
    } else if (!isStatic) {
        position += normal * overlap;
    } else if (!other.isStatic) {
        other.position -= normal * overlap;
    }

    // Step 3: Velocity Resolution (The Magic Bounce)
    glm::vec2 v_rel = velocity - other.velocity;
    float velAlongNormal = glm::dot(v_rel, normal);

    // Sanity Check: If they are moving apart, do nothing
    if (velAlongNormal > 0) {
        return;
    }

    // Calculate Impulse (j)
    float e = 0.8f; // Restitution
    if (velAlongNormal > -20.0f) {
        e = 0.0f; // Kill the bounce if they are barely colliding
    }
    
    // Treat static objects as having infinite mass (0 inverse mass)
    float invMass1 = isStatic ? 0.0f : (1.0f / mass);
    float invMass2 = other.isStatic ? 0.0f : (1.0f / other.mass);
    
    float j = -(1.0f + e) * velAlongNormal;
    j /= (invMass1 + invMass2);

    // Apply impulse to velocities
    glm::vec2 impulse = j * normal;
    if (!isStatic) velocity += impulse * invMass1;
    if (!other.isStatic) other.velocity -= impulse * invMass2;
}
