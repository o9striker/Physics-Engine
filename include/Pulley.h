#pragma once
#include "Particle.h"
#include <vector>

class Pulley {
public:
    size_t particleA;
    size_t particleB;
    size_t anchor;
    
    float totalStringLength;
    float stiffness;
    float damping;

    Pulley(size_t pA, size_t pB, size_t anc, float length, float k, float d);

    // Enforces the pulley length constraint
    void Update(std::vector<Particle>& particles);
};
