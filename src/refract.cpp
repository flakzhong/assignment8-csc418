#include "refract.h"
#include <iostream>

float fresnel(
    const Eigen::Vector3d & incident,
    const Eigen::Vector3d & normal,
    float n1,
    float n2)
{
    float cosi = cosi = incident.dot(normal);
    float etai = n1, etat = n2; 
    float kr;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    return kr;
} 

Eigen::Vector3d refract(
    const Eigen::Vector3d & incident,
    const Eigen::Vector3d & normal,
    float n1,
    float n2)
{
    float cosi = incident.dot(normal);
    float eta = n1/n2;
    Eigen::Vector3d n = normal;
    if (cosi < 0) { cosi = -cosi; } else { eta = 1/eta; n = -normal; }
    
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Eigen::Vector3d(0,0,0) : eta * incident + (eta * cosi - sqrtf(k)) * n; 
}
