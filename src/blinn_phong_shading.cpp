#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>
#include <algorithm>

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  Eigen::Vector3d rgb(0.0, 0.0 ,0.0);
  Eigen::Vector3d kd = objects[hit_id]->material->kd;
  Eigen::Vector3d ks = objects[hit_id]->material->ks;
  Eigen::Vector3d q = ray.origin + ray.direction*t;
  double p = objects[hit_id]->material->phong_exponent;
  for (int i = 0; i < lights.size(); i++) {
    Eigen::Vector3d l_i;
    double max_t;
    lights[i]->direction(q, l_i, max_t);
    Eigen::Vector3d I_i = lights[i]->I;
    
    Ray back_ray;
    back_ray.origin = q;
    back_ray.direction = l_i;
    int temp_hit_id;
    double temp_dist;
    Eigen::Vector3d temp_n;
    
    if (!first_hit(back_ray, 0.00001, objects, temp_hit_id, temp_dist, temp_n) || max_t < temp_dist) {
      rgb += (kd.array()*I_i.array()).matrix()*fmax(0.0, n.dot(l_i));
      Eigen::Vector3d h_i = l_i - ray.direction;
      h_i.normalize();
      rgb += (ks.array()*I_i.array()).matrix()*pow(fmax(0, n.dot(h_i)), p);
    }
  }
  return rgb;
  ////////////////////////////////////////////////////////////////////////////
}
