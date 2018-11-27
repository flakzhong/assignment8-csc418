#include "ray_tracer.h"
#include "pthread_args_struct.h"
#include <random>

void *ray_tracer(void *thread_args) {
    int height, width, thread_id, start;
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
    thread_id = curr_args->id;

    if (thread_id == 0) {
        start = 0;
    } else {
        start = thread_id*height/num_threads;
    }
    int stop = (thread_id + 1)*(height/num_threads);

    // For each pixel (i,j)
    for(unsigned i = start; i < stop; ++i) {
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
    }
}
