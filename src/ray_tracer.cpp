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

// void *ray_tracer(void *thread_args) {
//     int height, width, thread_id, start;
//     std::vector<unsigned char> *rgb_image;
//     std::vector< std::shared_ptr<Object> > objects;
//     Camera camera;
//     std::vector< std::shared_ptr<Light> > lights;


//     struct args_struct *curr_args;
//     curr_args = (struct args_struct *) thread_args;
//     height = curr_args->shared_params->height;
//     width = curr_args->shared_params->width;
//     rgb_image = curr_args->shared_params->rgb_image;
//     objects = curr_args->shared_params->objects;
//     lights = curr_args->shared_params->lights;
//     camera = curr_args->shared_params->camera;
//     int num_threads = curr_args->shared_params->num_threads;
//     thread_id = curr_args->id;

//     if (thread_id == 0) {
//         start = 0;
//     } else {
//         start = thread_id*height/num_threads;
//     }
//     int stop = (thread_id + 1)*(height/num_threads);

//     float jitterMatrix[4 * 2] = {
//       -1.0/4.0,  3.0/4.0,
//        3.0/4.0,  1.0/3.0,
//       -3.0/4.0, -1.0/4.0,
//        1.0/4.0, -3.0/4.0,
//     };


//     // For each pixel (i,j)
//     for(unsigned i = start; i < stop; ++i) {
//         for(unsigned j=0; j<width; ++j) {
        
//         Eigen::Vector3d rgb(0,0,0);
//         for (int sample = 0; sample < 4; sample++) {
//             Eigen::Vector3d temp(0, 0, 0);
//             // Compute viewing ray
//             Ray ray;
//             viewing_ray(camera,i + jitterMatrix[2*sample],j + jitterMatrix[2*sample+1],width,height,ray);

//             // Shoot ray and collect color
//             raycolor(ray,1.0,objects,lights,0,temp);
//             rgb += temp;
//         }
//         rgb /= 4.0;
//         auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
//         (*rgb_image)[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
//         (*rgb_image)[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
//         (*rgb_image)[2+3*(j+width*i)] = 255.0*clamp(rgb(2));
        
//       }
//     }
// }