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
    Eigen::Vector3d hit_point = ray.origin + t*ray.direction;
    Ray rflt;
    rflt.origin = hit_point;
    rflt.direction = reflect(ray.direction, n);
    Eigen::Vector3d r_color;
    
    rgb = objects[hit_id]->material->ka;
    rgb += blinn_phong_shading(ray, hit_id, t, n, objects, lights);
    if (pow(objects[hit_id]->material->km(0), num_recursive_calls) > 0.00001 
      && raycolor(rflt, 0.00001, objects, lights, num_recursive_calls + 1, r_color)) {
      rgb += (r_color.array()*objects[hit_id]->material->km.array()).matrix();
    }

    if (objects[hit_id]->material->transparency) {
      Eigen::Vector3d refract_color(0,0,0);
      float kr = fresnel(ray.direction, n, 1, 1.5);
      if (kr < 1) {
        bool outside = ray.direction.dot(n) < 0;
        Ray rfrct;
        if (outside) {
          rfrct.origin = hit_point - 0.00001*n;
        } else {
          rfrct.origin = hit_point + 000001*n;
        }
        rfrct.direction = refract(ray.direction, n, 1, 1.5);
        raycolor(rfrct, 0.00001, objects, lights, num_recursive_calls + 1, refract_color);
      }

      rgb = rgb*kr +  refract_color*(1 - kr);
    }
    return true;
  }
  return false;
  ////////////////////////////////////////////////////////////////////////////
}
