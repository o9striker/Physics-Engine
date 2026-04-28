#include "Spring.h"

Spring::Spring(int pA, int pB, float length, float k, float d) {
    particleA = pA;
    particleB = pB;
    restLength = length;
    stiffness = k;
    damping = d;
}

void Spring::Update(std::vector<Particle>& particles) {
    // 1. Get references to the actual particles using their vector indices
    Particle& pA = particles[particleA];
    Particle& pB = particles[particleB];

    // 2. Calculate the distance vector (from A to B)
    glm::vec2 diff = pB.position - pA.position;
    
    // 3. Calculate current length
    float currentLength = glm::length(diff);
    
    // Safeguard to prevent division by zero if particles are exactly on top of each other
    if (currentLength == 0.0f) {
        return; 
    }

    // 4. Calculate Displacement (x)
    float x = currentLength - restLength;

    // 5. Calculate Spring Force Magnitude (Hooke's Law: F = k * x)
    // Note: since diff is B - A, a positive x (stretched) means A needs to be pulled towards B,
    // and B pulled towards A. So Force on A is + direction, Force on B is - direction.
    float springForce = stiffness * x;

    // 6. Calculate Damping Force
    // We project the relative velocity onto the spring's normalized direction
    glm::vec2 dir = diff / currentLength;
    glm::vec2 v_rel = pB.velocity - pA.velocity;
    float velAlongSpring = glm::dot(v_rel, dir);
    
    // Damping resists the relative motion
    float dampingForce = damping * velAlongSpring;

    // 7. Calculate Total Force Magnitude
    float totalForce = springForce + dampingForce;

    // 8. Convert to Vector
    glm::vec2 forceVec = dir * totalForce;

    // 9. Apply Force
    // A is pulled towards B (positive dir)
    // B is pulled towards A (negative dir)
    pA.ApplyForce(forceVec);
    pB.ApplyForce(-forceVec);
}
