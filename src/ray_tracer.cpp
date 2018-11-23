#include "ray_tracer.h"
#include "pthread_args_struct.h"

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

    int num_threads = curr_args->num_threads;
    if (thread_id == 0) {
        start = 0;
    } else {
        start = thread_id*height/num_threads;
    }
    int stop = (thread_id + 1)*(height/num_threads);

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
}