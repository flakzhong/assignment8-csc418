#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "pthread_args_struct.h"
#include <random>
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <pthread.h>
#include <thread>


const int num_threads = 40;
volatile int available_threads = num_threads;
int buffer[num_threads];
volatile int buffer_head = 0;
volatile int buffer_tail = 0;
volatile int buffer_n = 0;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

void *ray_tracer(void *thread_args) {
  int height, width;
  std::vector<unsigned char> *rgb_image;
  std::vector< std::shared_ptr<Object> > objects;
  Camera camera;
  std::vector< std::shared_ptr<Light> > lights;


  struct args_struct *curr_args;
  curr_args = (struct args_struct *) thread_args;
  height = curr_args->shared_params->height;
  width = curr_args->shared_params->width;
  rgb_image = curr_args->shared_params->rgb_image;
  objects = curr_args->shared_params->objects;
  lights = curr_args->shared_params->lights;
  camera = curr_args->shared_params->camera;
  int num_threads = curr_args->shared_params->num_threads;
  int i = curr_args->id;

  for(unsigned j=0; j<width; ++j) {
    Ray ray;
    viewing_ray(camera,i,j,width,height,ray);

    Eigen::Vector3d rgb(0,0,0);
    int monte_carlo_dof = 0;
    if (monte_carlo_dof) {
        // depth of field
        double focal_length = camera.d;
        double aperture = focal_length/camera.fstops;
        Eigen::Vector3d focal_point = ray.origin + focal_length*ray.direction;

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 eng(rd()); // seed the generator
        std::uniform_real_distribution<> distr(-0.001, 0.001); // define the range
        for (int k = 0; k < monte_carlo_dof; k++) {
            for(int l = 0; l < 3; l++) {
                float rnd = distr(eng)*aperture;
                ray.origin[l] += rnd;
                ray.direction = (focal_point - ray.origin).normalized();
            }
            Eigen::Vector3d temp(0,0,0);

            // Shoot ray and collect color
            raycolor(ray,1.0,objects,lights,0,temp);
            rgb += temp;
        }
        rgb /= monte_carlo_dof;
    } else {
        raycolor(ray,1.0,objects,lights,0,rgb);
    }
    
    auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
    (*rgb_image)[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
    (*rgb_image)[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
    (*rgb_image)[2+3*(j+width*i)] = 255.0*clamp(rgb(2));
  }
  pthread_mutex_lock(&running_mutex);
  available_threads++;
  buffer[buffer_tail] = i;
  buffer_tail = (buffer_tail + 1)%num_threads;
  buffer_n++;
  pthread_mutex_unlock(&running_mutex);
}

int main(int argc, char * argv[])
{
  Camera camera;
  std::vector< std::shared_ptr<Object> > objects;
  std::vector< std::shared_ptr<Light> > lights;
  // Read a camera and scene description from given .json file
  read_json(
    argc<=1?"../scene/test.json":argv[1],
    camera,
    objects,
    lights);

  int width =  640;
  int height = 360;
  pthread_t threads[height];
  std::vector<unsigned char> rgb_image(3*width*height);


  struct args_struct args[height];
  struct shared_params shared_params;

  shared_params.height = height;
  shared_params.width = width;
  shared_params.rgb_image = &rgb_image;
  shared_params.objects = objects;
  shared_params.lights = lights;
  shared_params.camera = camera;
  shared_params.num_threads = num_threads;

  for (int i = 0; i < height; i++) {
    args[i].id = i;
    args[i].shared_params = &shared_params;
    pthread_mutex_lock(&running_mutex);
    available_threads--;
    pthread_mutex_unlock(&running_mutex);
    
    pthread_create(&threads[i], NULL, ray_tracer, &args[i]);

    if (i != height - 1) {
      while (available_threads == 0){;}
    }
  
    pthread_mutex_lock(&running_mutex);
    while (buffer_n != 0) {
      pthread_join(threads[buffer[buffer_head]], NULL);
      buffer_head = (buffer_head + 1)%num_threads;
      buffer_n--;
    }
    pthread_mutex_unlock(&running_mutex);
  }

  while (buffer_n !=0) {
    pthread_join(threads[buffer[buffer_head]], NULL);
    buffer_head = (buffer_head + 1)%num_threads;
    buffer_n--;
  }

  write_ppm("rgb.ppm",rgb_image,width,height,3);
  return system("convert rgb.ppm rgb.png");
}
