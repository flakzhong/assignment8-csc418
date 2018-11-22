#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "set_color.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <pthread.h>

#define NUM_THREADS 8


int main(int argc, char * argv[])
{
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

  float jitterMatrix[4 * 2] = {
    -1.0/4.0,  3.0/4.0,
     3.0/4.0,  1.0/3.0,
    -3.0/4.0, -1.0/4.0,
     1.0/4.0, -3.0/4.0,
};

  // For each pixel (i,j)
  for(unsigned i=0; i<height; ++i) 
  {
    for(unsigned j=0; j<width; ++j)
    {
      rgb_image[0+3*(j+width*i)] = 0;
      rgb_image[1+3*(j+width*i)] = 0;
      rgb_image[2+3*(j+width*i)] = 0;
      for (int sample = 0; sample < 4; sample++) {
        // Set background color
        auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
        Eigen::Vector3d rgb = set_color(camera, i, j, width, height, objects, lights);
        rgb_image[0+3*(j+width*i)] += 255.0*clamp(rgb(0));
        rgb_image[1+3*(j+width*i)] += 255.0*clamp(rgb(1));
        rgb_image[2+3*(j+width*i)] += 255.0*clamp(rgb(2));
      }
      rgb_image[0+3*(j+width*i)] /= 4.0;
      rgb_image[1+3*(j+width*i)] /= 4.0;
      rgb_image[2+3*(j+width*i)] /= 4.0;
    }
  }

  write_ppm("rgb.ppm",rgb_image,width,height,3);
}
