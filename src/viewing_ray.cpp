#include "viewing_ray.h"

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ray.origin = camera.e;
  double j_unit = camera.width/width;
  double i_unit = camera.height/height;
  Eigen::Vector3d focal_pt = camera.w*camera.d*(-1);
  double plane_center_x = width/2.0 - 0.5;
  double plane_center_y = height/2.0 - 0.5;
  ray.direction = (j - plane_center_x)*j_unit*camera.u +
    (plane_center_y - i)*i_unit*camera.v + focal_pt;
  ray.direction.normalize();
  ////////////////////////////////////////////////////////////////////////////
}
