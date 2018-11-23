#ifndef RAY_TRACER_H
#define RAY_TRACER_H
#endif
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "pthread_args_struct.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <pthread.h>


void *ray_tracer(void *);

void *ray_tracer(void *thread_args) {
    int height, width, thread_id, start;
    std::vector<unsigned char> *rgb_image;
    std::vector< std::shared_ptr<Object> > objects;
    Camera camera;
    std::vector< std::shared_ptr<Light> > lights;


    struct args_struct *curr_args;
    curr_args = (struct args_struct *) thread_args;
    height = curr_args->height;
    width = curr_args->width;
    rgb_image = curr_args->rgb_image;
    objects = curr_args->objects;
    lights = curr_args->lights;
    camera = curr_args->camera;
    thread_id = curr_args->id;


    if (thread_id == 0) {
        start = 0;
    } else {
        start = thread_id*height/8;
    }
    int stop = (thread_id + 1)*(height/8);

    // For each pixel (i,j)
    for(unsigned i = start; i < stop; ++i) {
        for(unsigned j=0; j<width; ++j) {
        Eigen::Vector3d rgb(0,0,0);
        Ray ray;
        viewing_ray(camera,i,j,width,height,ray);
        
        // Shoot ray and collect color
        raycolor(ray,1.0,objects,lights,0,rgb);
        auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
        (*rgb_image)[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
        (*rgb_image)[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
        (*rgb_image)[2+3*(j+width*i)] = 255.0*clamp(rgb(2));
        
      }
    }
    // std::cout << "thread:" << thread_id  << "finished its iteration" << "\n";
}

int main(int argc, char * argv[])
{
  const int num_threads = 8;
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
    pthread_create(&threads[k], NULL, ray_tracer, (void *) &args[k]);
  }

  for (int k = 0; k < 8; k++)
        pthread_join(threads[k], NULL);

  int i = 180, j = 320;
  std::cout << rgb_image[0+3*(j+width*i)] << " " << rgb_image[1+3*(j+width*i)]
    << " " << rgb_image[2+3*(j+width*i)] << "\n";

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
