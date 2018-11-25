#ifndef REFRACT_H
#define REFRACT_H
#include <Eigen/core>

Eigen::Vector3d refract(
    const Eigen::Vector3d & incident,
    const Eigen::Vector3d & normal,
    float n1,
    float n2
);
#endif