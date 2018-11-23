#ifndef RAY_TRACER_H
#define RAY_TRACER_H
#endif
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include <Eigen/Core>
#include <vector>
#include <pthread.h>

void *ray_tracer(void *);