#include "refract.h"
#include <iostream>

Eigen::Vector3d refract(
    const Eigen::Vector3d & incident,
    const Eigen::Vector3d & normal,
    float n1,
    float n2)
{
    float eta = n1/n2;
    float cosi = normal.dot(incident);
    Eigen::Vector3d n = normal;
    if (cosi < 0) {
        cosi = -cosi;
    } else {
        eta = 1/eta;
        n = -normal;
    }
    float k = 1 - eta*eta*(1 - cosi*cosi);
    return k < 0 ?  Eigen::Vector3d(0,0,0): eta*incident + (eta*cosi - sqrt(k))*n;
}
