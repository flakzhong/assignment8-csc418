#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "ray_tracer.h"

struct args_struct {
    int height;
    int width;
    Camera camera;
    std::vector< std::shared_ptr<Light> > lights;
    std::vector<unsigned char> rgb_image;
    std::vector< std::shared_ptr<Object> > objects;
  };