#include "Plane.h"
#include "Ray.h"
#include <iostream>
bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  Eigen::Vector3d d = ray.direction;
  Eigen::Vector3d e = ray.origin;
  double temp_t = ((point - e).dot(normal))/d.dot(normal);
  if (temp_t < min_t) {
    return false;
  } else {
    t = temp_t;
    n = normal;
    return true;
  }
  ////////////////////////////////////////////////////////////////////////////
}

