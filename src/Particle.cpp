#include "Particle.h"
#include <algorithm>

const float GRAVITY = 981.0f; // Pixels per second squared

Particle::Particle(float x, float y, float m, float r, uint32_t c) {
    shape = CIRCLE;
    position = glm::vec2(x, y);
    velocity = glm::vec2(0.0f, 0.0f);
    acceleration = glm::vec2(0.0f, 0.0f);
    forceAccumulator = glm::vec2(0.0f, 0.0f);
    mass = m;
    radius = r;
    width = 0.0f;
    height = 0.0f;
    color = c;
    isStatic = false;
}

Particle::Particle(float x, float y, float m, float w, float h, uint32_t c) {
    shape = BOX;
    position = glm::vec2(x, y);
    velocity = glm::vec2(0.0f, 0.0f);
    acceleration = glm::vec2(0.0f, 0.0f);
    forceAccumulator = glm::vec2(0.0f, 0.0f);
    mass = m;
    radius = 0.0f;
    width = w;
    height = h;
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

    float halfW = (shape == BOX) ? width / 2.0f : radius;
    float halfH = (shape == BOX) ? height / 2.0f : radius;

    // Floor Collision
    if (position.y + halfH >= screenHeight) {
        position.y = (float)screenHeight - halfH;
        if (velocity.y < 30.0f) {
            velocity.y = 0.0f; // Put to sleep vertically
        } else {
            velocity.y *= -0.8f; // Bounce with restitution
        }
    }

    // Left/Right Wall Collision
    if (position.x - halfW <= 0) {
        position.x = halfW;
        velocity.x *= -0.8f;
    } else if (position.x + halfW >= screenWidth) {
        position.x = (float)screenWidth - halfW;
        velocity.x *= -0.8f;
    }
}

void Particle::ResolveCollision(Particle& other) {
    glm::vec2 normal(0.0f, 0.0f);
    float overlap = 0.0f;
    bool collided = false;

    if (shape == CIRCLE && other.shape == CIRCLE) {
        // Step 1: Detection (The Pythagorean Check)
        float dist = glm::distance(position, other.position);
        float sumRadii = radius + other.radius;

        if (dist < sumRadii) {
            collided = true;
            if (dist == 0.0f) {
                position.x += 0.01f;
                dist = glm::distance(position, other.position);
            }
            overlap = sumRadii - dist;
            normal = glm::normalize(position - other.position);
        }
    } else if (shape == BOX && other.shape == BOX) {
        // Box vs Box (AABB)
        float leftA = position.x - width / 2.0f;
        float rightA = position.x + width / 2.0f;
        float topA = position.y - height / 2.0f;
        float bottomA = position.y + height / 2.0f;

        float leftB = other.position.x - other.width / 2.0f;
        float rightB = other.position.x + other.width / 2.0f;
        float topB = other.position.y - other.height / 2.0f;
        float bottomB = other.position.y + other.height / 2.0f;

        if (rightA > leftB && leftA < rightB && bottomA > topB && topA < bottomB) {
            collided = true;
            
            float penLeft = rightA - leftB;
            float penRight = rightB - leftA;
            float penTop = bottomA - topB;
            float penBottom = bottomB - topA;

            float minPen = std::min({penLeft, penRight, penTop, penBottom});
            overlap = minPen;

            if (minPen == penLeft) {
                normal = glm::vec2(-1.0f, 0.0f);
            } else if (minPen == penRight) {
                normal = glm::vec2(1.0f, 0.0f);
            } else if (minPen == penTop) {
                normal = glm::vec2(0.0f, -1.0f);
            } else if (minPen == penBottom) {
                normal = glm::vec2(0.0f, 1.0f);
            }
        }
    } else {
        // Circle vs Box
        Particle* circle = (shape == CIRCLE) ? this : &other;
        Particle* box = (shape == BOX) ? this : &other;

        float closestX = std::clamp(circle->position.x, box->position.x - box->width / 2.0f, box->position.x + box->width / 2.0f);
        float closestY = std::clamp(circle->position.y, box->position.y - box->height / 2.0f, box->position.y + box->height / 2.0f);

        float distanceX = circle->position.x - closestX;
        float distanceY = circle->position.y - closestY;
        float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        if (distanceSquared < (circle->radius * circle->radius)) {
            collided = true;
            float distance = std::sqrt(distanceSquared);
            
            if (distance == 0.0f) {
                // Circle center is inside the box
                // Push it out to the nearest edge
                float penLeft = circle->position.x - (box->position.x - box->width / 2.0f);
                float penRight = (box->position.x + box->width / 2.0f) - circle->position.x;
                float penTop = circle->position.y - (box->position.y - box->height / 2.0f);
                float penBottom = (box->position.y + box->height / 2.0f) - circle->position.y;
                
                float minPen = std::min({penLeft, penRight, penTop, penBottom});
                overlap = circle->radius + minPen; // Needs to be pushed out by radius + distance to edge
                
                if (minPen == penLeft) normal = glm::vec2(-1.0f, 0.0f);
                else if (minPen == penRight) normal = glm::vec2(1.0f, 0.0f);
                else if (minPen == penTop) normal = glm::vec2(0.0f, -1.0f);
                else if (minPen == penBottom) normal = glm::vec2(0.0f, 1.0f);
                
                if (circle == &other) normal = -normal;
            } else {
                overlap = circle->radius - distance;
                normal = glm::vec2(distanceX / distance, distanceY / distance);
                if (circle == &other) normal = -normal;
            }
        }
    }

    if (!collided) return;

    // Step 2: Positional Correction (Anti-Squish)
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
