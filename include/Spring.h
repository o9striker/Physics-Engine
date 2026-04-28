#pragma once
#include <cstddef>

struct Spring {
    size_t particleA;
    size_t particleB;
    float restLength;
    float stiffness;
};
