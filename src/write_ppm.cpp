#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  std::ofstream output;
  output.open(filename.c_str(), std::ofstream::binary);

  // path is invalid
  if (!output.good()) {
    return false;
  }

  // write header
  if (num_channels == 1) {
    output << "P5 " << width << " " << height  << " 255\n";
  } else {
    output << "P6 " << width << " " << height  << " 255\n";
  }


  for (int i = 0; i < width*height*num_channels; i++) {
    output << data[i];
  }

  output.close();
  return true;
  ////////////////////////////////////////////////////////////////////////////
}
