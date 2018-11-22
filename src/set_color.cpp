#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "set_color.h"
#include <Eigen/Core>
#include <vector>
#include <limits>

Eigen::Vector3d set_color (
    const Camera & camera,
    const int i,
    const int j,
    const int width,
    const int height,
    const std::vector< std::shared_ptr<Object> > & objects,
    const std::vector< std::shared_ptr<Light> > & lights)
{
    Eigen::Vector3d rgb(0,0,0);
    Ray ray;
    viewing_ray(camera,i,j,width,height,ray);
    raycolor(ray,1.0,objects,lights,0,rgb);
    return rgb;
}