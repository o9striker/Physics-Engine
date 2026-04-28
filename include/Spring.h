#pragma once
#include "Particle.h"
#include <vector>

class Spring {
public:
    int particleA;
    int particleB;
    float restLength;
    float stiffness;
    float damping;

    // Constructor
    Spring(int pA, int pB, float length, float k, float d);

    // Applies Hooke's Law and Damping to the particles
    void Update(std::vector<Particle>& particles);
};
