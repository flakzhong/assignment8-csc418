#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Geometry>
#include <iostream>

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  Eigen::Vector3d d = ray.direction;
  Eigen::Vector3d e = ray.origin;
  Eigen::Vector3d a = std::get<0>(corners);
  Eigen::Vector3d b = std::get<1>(corners);
  Eigen::Vector3d c = std::get<2>(corners);
  Eigen::Vector3d normal = (a - b).cross(a - c);
  Eigen::Vector3d point = c;

  double temp_t = ((point - e).dot(normal))/d.dot(normal);
  if (temp_t < min_t) {
    return false;
  }

  Eigen::Vector3d intersection = e + temp_t*d;
  double a1 = (intersection - a).cross(intersection - b).norm();
  double a2 = (intersection - b).cross(intersection - c).norm();
  double a3 = (intersection - a).cross(intersection - c).norm();
  if ((a - b).cross(a - c).norm() + std::numeric_limits<double>::epsilon() < a1 + a2 + a3) {
    return false;
  } else {
    t = temp_t;
    n = normal;
    return true;
  }
  ////////////////////////////////////////////////////////////////////////////
}


