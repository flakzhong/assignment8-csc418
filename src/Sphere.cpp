#include "Sphere.h"
#include "Ray.h"

bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  Eigen::Vector3d d = ray.direction;
  Eigen::Vector3d e = ray.origin;
  double delta = pow(d.dot(e - center), 2)
    - (d.dot(d))*((e - center).dot(e - center) - radius*radius);
  if (delta >= 0) {
    double t1 = (((-1)*d).dot(e - center) + sqrt(delta))/(d.dot(d));
    double t2 = (((-1)*d).dot(e - center) - sqrt(delta))/(d.dot(d));
    if (t1 < min_t) {
      return false;
    }

    Eigen::Vector3d p;
    if (t2 < min_t) {
      t = t1;
      p = e + t*d;
      n = p - center;
    } else {
      t = t2;
      p = e + t*d;
      n = p - center;
    }
    return true;
  } else {
    return false;
  }
  ////////////////////////////////////////////////////////////////////////////
}

