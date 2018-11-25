#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"
#include "refract.h"
#include <iostream>


bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  int hit_id;
  double t;
  Eigen::Vector3d n;
  if (first_hit(ray, min_t, objects, hit_id, t, n)) {
    float transparency = objects[hit_id]->material->transparency;
    if (transparency) {
      Eigen::Vector3d refract_rgb(0, 0, 0);
      Ray refract_ray;
      refract_ray.origin = q.origin + 0.0001*n;
      refract_ray.direction = refract(q.origin, n, 1.0, 1.5);
      if (pow(objects[hit_id]->material->km(0), num_recursive_calls) > 0.00001
        && raycolor(refract_ray, 0.00001, objects, lights, num_recursive_calls + 1, refract_rgb)) {
          rgb = refract_rgb;
      }
    } else {
      Ray q;
      q.origin = ray.origin + t*ray.direction;
      q.direction = reflect(ray.direction, n);
      Eigen::Vector3d r_color;
      
      rgb = objects[hit_id]->material->ka;
      rgb += blinn_phong_shading(ray, hit_id, t, n, objects, lights);
      if (pow(objects[hit_id]->material->km(0), num_recursive_calls) > 0.00001 
        && raycolor(q, 0.00001, objects, lights, num_recursive_calls + 1, r_color)) {
        rgb += (r_color.array()*objects[hit_id]->material->km.array()).matrix();
      }
    }
    return true;
  }
  return false;
  ////////////////////////////////////////////////////////////////////////////
}
