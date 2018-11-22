#ifndef SET_COLOR_H
#define SET_COLOR_H
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "viewing_ray.h"
#include "raycolor.h"
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
    const std::vector< std::shared_ptr<Light> > & lights);
#endif