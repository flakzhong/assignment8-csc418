#include "TriangleSoup.h"
#include "Triangle.h"
#include "first_hit.h"
bool TriangleSoup::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  double closest_t = -1;
  Eigen::Vector3d n_of_hit;
  int hit_index = -1;

  for (int i = 0; i < triangles.size(); i++) {
    double temp_t;
    Eigen::Vector3d temp_n;
    if (triangles[i]->intersect(ray, min_t, temp_t, temp_n)) {
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
    n = n_of_hit;
    n.normalize();
    return true;
  } else {
    return false;
  }
  ////////////////////////////////////////////////////////////////////////////
}
