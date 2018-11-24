#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "pthread_args_struct.h"
#include "ray_tracer.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <pthread.h>
#include <thread>


int main(int argc, char * argv[])
{
  const int num_threads = std::thread::hardware_concurrency();
  pthread_t threads[num_threads];

  Camera camera;
  std::vector< std::shared_ptr<Object> > objects;
  std::vector< std::shared_ptr<Light> > lights;
  // Read a camera and scene description from given .json file
  read_json(
    argc<=1?"../shared/data/sphere-and-plane.json":argv[1],
    camera,
    objects,
    lights);

  int width =  640;
  int height = 360;
  std::vector<unsigned char> rgb_image(3*width*height);


  struct args_struct args[num_threads];
  struct shared_params shared_params;

  shared_params.height = height;
  shared_params.width = width;
  shared_params.rgb_image = &rgb_image;
  shared_params.objects = objects;
  shared_params.lights = lights;
  shared_params.camera = camera;
  shared_params.num_threads = num_threads;

  for (int k = 0; k < num_threads; k++) {
    args[k].id = k;
    args[k].shared_params = &shared_params;
    pthread_create(&threads[k], NULL, ray_tracer, (void *) &args[k]);
  }

  for (int k = 0; k < num_threads; k++)
        pthread_join(threads[k], NULL);

  write_ppm("rgb.ppm",rgb_image,width,height,3);
  return system("convert rgb.ppm rgb.png");
}
