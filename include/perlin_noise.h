#include "smooth_step.h"
#include "random_direction.h"
#include <Eigen/Core>


// Given a 3d position as a seed, compute an even smoother procedural noise
// value. "Improving Noise" [Perlin 2002].
//
// Inputs:
//   st  3D seed
// Values between  -½ and ½ ?
//
// expects: random_direction, improved_smooth_step
#define lerp(a0, a1, w) ((1.0 - w)*a0 + w*a1)
float perlin_noise(Eigen::Vector3d st) 
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 
  Eigen::Vector3d xyz0 = Eigen::Vector3d(floor(st[0]), floor(st[1]), floor(st[2]));
  Eigen::Vector3d w = smooth_step(st - xyz0);

  float n0, n1, ix0, ix1, iy0, iy1;

  n0 = (st - xyz0).dot(random_direction(xyz0));
  Eigen::Vector3d xyz1 = xyz0 + Eigen::Vector3d(1, 1, 1);
  Eigen::Vector3d temp = Eigen::Vector3d(xyz1[0], xyz0[1], xyz0[2]);
  n1 = (st - temp).dot(random_direction(temp));
  ix0 = lerp(n0, n1, w[0]);

  temp = Eigen::Vector3d(xyz0[0], xyz1[1], xyz0[2]);
  n0 = (st - temp).dot(random_direction(temp));
  temp = Eigen::Vector3d(xyz1[0], xyz1[1], xyz0[2]);
  n1 = (st - temp).dot(random_direction(temp));
  ix1 = lerp(n0, n1, w[0]);
  iy0 = lerp(ix0, ix1, w[1]);

  temp = Eigen::Vector3d(xyz0[0], xyz0[1], xyz1[2]);
  n0 = (st - temp).dot(random_direction(temp));
  temp = Eigen::Vector3d(xyz1[0], xyz0[1], xyz1[2]);
  n1 = (st - temp).dot(random_direction(temp));
  ix0 = lerp(n0, n1, w[0]);

  temp = Eigen::Vector3d(xyz0[0], xyz1[1], xyz1[2]);
  n0 = (st - temp).dot(random_direction(temp));
  temp = xyz1;
  n1 = (st - temp).dot(random_direction(temp));
  ix1 = lerp(n0, n1, w[0]);
  iy1 = lerp(ix0, ix1, w[1]);
  
  return lerp(iy0, iy1, w[2])/sqrt(3);
  /////////////////////////////////////////////////////////////////////////////
}

