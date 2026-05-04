#include "Pulley.hpp"

Pulley::Pulley(size_t pA, size_t pB, size_t anc, float length, float k, float d) {
    particleA = pA;
    particleB = pB;
    anchor = anc;
    totalStringLength = length;
    stiffness = k;
    damping = d;
}

void Pulley::Update(std::vector<Particle>& particles) {
    Particle& pA = particles[particleA];
    Particle& pB = particles[particleB];
    Particle& pAnc = particles[anchor];

    // 1. Calculate distance vectors pointing towards the anchor
    glm::vec2 diffA = pAnc.position - pA.position;
    glm::vec2 diffB = pAnc.position - pB.position;

    float dA = glm::length(diffA);
    float dB = glm::length(diffB);

    // 2. The Slack Check
    float currentLength = dA + dB;
    if (currentLength <= totalStringLength) {
        return; 
    }

    // 3. Tension Force (Hooke's Law)
    float x = currentLength - totalStringLength;
    float tension = stiffness * x;

    // 4. Damping Force
    // We calculate how fast the string is expanding/contracting
    glm::vec2 dirA = diffA / dA;
    glm::vec2 dirB = diffB / dB;

    // Projection of velocities onto the string directions
    // Note: dot(v, dir) > 0 means the particle is moving TOWARDS the anchor
    float vA = glm::dot(pA.velocity, dirA);
    float vB = glm::dot(pB.velocity, dirB);

    // The expansion rate is the negative sum of these projections
    float expansionRate = -(vA + vB);
    float dampingForce = damping * expansionRate;

    // 5. Total Tension
    float totalTension = tension + dampingForce;
    if (totalTension < 0) totalTension = 0; // String cannot push

    // 6. Apply Forces towards the anchor
    pA.ApplyForce(dirA * totalTension);
    pB.ApplyForce(dirB * totalTension);
}
