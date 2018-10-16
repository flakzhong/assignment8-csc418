#include "first_hit.h"

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  double closest_t = -1;
  Eigen::Vector3d n_of_hit;
  int hit_index = -1;

  for (int i = 0; i < objects.size(); i++) {
    double temp_t;
    Eigen::Vector3d temp_n;
    if (objects[i]->intersect(ray, min_t, temp_t, temp_n)) {
      if (closest_t == -1 || temp_t < closest_t) {
        closest_t = temp_t;
        hit_index = i;
        n_of_hit = temp_n;
      }
    }
  }

  // there's at least one hit pt.
  if (hit_index != -1) {
    t = closest_t;
    hit_id = hit_index;
    n = n_of_hit;
    n.normalize();
    return true;
  } else {
    return false;
  }
  ////////////////////////////////////////////////////////////////////////////
}
