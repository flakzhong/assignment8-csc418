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

  for (int k = 0; k < num_threads; k++) {
    args[k].height = height;
    args[k].width = width;
    args[k].rgb_image = &rgb_image;
    args[k].objects = objects;
    args[k].lights = lights;
    args[k].camera = camera;
    args[k].id = k;
    args[k].num_threads = num_threads;
    pthread_create(&threads[k], NULL, ray_tracer, (void *) &args[k]);
  }

  for (int k = 0; k < num_threads; k++)
        pthread_join(threads[k], NULL);

  write_ppm("rgb.ppm",rgb_image,width,height,3);
  return 0;
}




  // float jitterMatrix[4 * 2] = {
  //   -1.0/4.0,  3.0/4.0,
  //    3.0/4.0,  1.0/3.0,
  //   -3.0/4.0, -1.0/4.0,
  //    1.0/4.0, -3.0/4.0,
  // };

  // // For each pixel (i,j)
  // for(unsigned i=0; i<height; ++i) 
  // {
  //   for(unsigned j=0; j<width; ++j)
  //   {
  //     Eigen::Vector3d rgb(0,0,0);
  //     Ray ray;
  //     viewing_ray(camera,i,j,width,height,ray);
      
  //     // Shoot ray and collect color
  //     raycolor(ray,1.0,objects,lights,0,rgb);
  //     auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
  //     rgb_image[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
  //     rgb_image[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
  //     rgb_image[2+3*(j+width*i)] = 255.0*clamp(rgb(2));

  //   //   Eigen::Vector3d rgb(0,0,0);
  //   //   for (int sample = 0; sample < 4; sample++) {
  //   //     Eigen::Vector3d temp(0, 0, 0);
  //   //     // Compute viewing ray
  //   //     Ray ray;
  //   //     viewing_ray(camera,i + jitterMatrix[2*sample],j + jitterMatrix[2*sample+1],width,height,ray);
        
  //   //     // Shoot ray and collect color
  //   //     raycolor(ray,1.0,objects,lights,0,temp);
  //   //     rgb += temp;
  //   //   }

  //   //   rgb /= 4.0;
  //   //   auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
  //   //   rgb_image[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
  //   //   rgb_image[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
  //   //   rgb_image[2+3*(j+width*i)] = 255.0*clamp(rgb(2));

  //   // }
  //   }
  // }
