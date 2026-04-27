#pragma once
#include <glm/glm.hpp>

class Particle {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass;

    // Constructor to set up a new particle
    Particle(float x, float y, float m);

    // The function that will eventually move the particle
    void Update(float deltaTime);
};
