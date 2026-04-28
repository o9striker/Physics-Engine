#pragma once
#include "Particle.h"
#include <vector>
#include <cstddef>

class Spring {
public:
    size_t particleA;
    size_t particleB;
    float restLength;
    float stiffness;
    float damping;

    // Constructor
    Spring(size_t pA, size_t pB, float length, float k, float d);

    // Applies Hooke's Law and Damping to the particles
    void Update(std::vector<Particle>& particles);
};
