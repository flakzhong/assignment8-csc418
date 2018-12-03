#include "ray_intersect_box.h"
#include <iostream>

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  double & t)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  double max_t = 9999;
  double txmin, txmax, tymin, tymax, tzmin, tzmax;
  double x_e = ray.origin[0], y_e = ray.origin[1], z_e = ray.origin[2],
    x_d = ray.direction[0], y_d = ray.direction[1], z_d = ray.direction[2];
  double xmin = box.min_corner[0], xmax = box.max_corner[0],
    ymin = box.min_corner[1], ymax = box.max_corner[1],
    zmin = box.min_corner[2], zmax = box.max_corner[2];
  double a = 1/x_d;
  if (a >= 0) {
    txmin = a*(xmin - x_e);
    txmax = a*(xmax - x_e);
  } else {
    txmax = a*(xmin - x_e);
    txmin = a*(xmax - x_e);
  }
  a = 1/y_d;
  if (a >= 0) {
    tymin = a*(ymin - y_e);
    tymax = a*(ymax - y_e);
  } else {
    tymax = a*(ymin - y_e);
    tymin = a*(ymax - y_e);
  }
  a = 1/z_d;
  if (z_d >= 0) {
    tzmin = a*(zmin - z_e);
    tzmax = a*(zmax - z_e);
  } else {
    tzmax = a*(zmin - z_e);
    tzmin = a*(zmax - z_e);
  }

  if (txmin > tymax || tymin > txmax || tzmin > tzmax) {
    return false;
  } else {
    double temp_min, temp_max;
    temp_min = (txmin < tymin)?tymin:txmin;
    temp_max = (txmax < tymax)?txmax:tymax;
    if (temp_max <= temp_min) {
      return false;
    }

    temp_min = (temp_min < tzmin)?tzmin:temp_min;
    temp_max = (temp_max < tzmax)?temp_max:tzmax;
    if (temp_max <= temp_min) {
      return false;
    }
    
    temp_min = (temp_min < min_t)?min_t:temp_min;
    temp_max = (temp_max < max_t)?temp_max:max_t;
    if (temp_max <= temp_min) {
      return false;
    } else {
      t = temp_min;
      return true;
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
