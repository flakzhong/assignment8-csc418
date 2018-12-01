#include <Eigen/Core>
// Generate a pseudorandom 2D vector based on a 2D or 3D seed.//
// https://thebookofshaders.com/edit.php#11/2d-gnoise.frag
//
// Inputs:
//   st  2D seed
// Returns 2D random point in [0,1]²
#define M_PI 3.14159265358979323846 

float fract(float f) {
  double garbage;
  return modf(f, &garbage);
}

Eigen::Vector2d random2(Eigen::Vector2d st){
  st = Eigen::Vector2d(st.dot(Eigen::Vector2d(127.1,311.7)),
            st.dot(Eigen::Vector2d(269.5,183.3)));
  float temp0 = fract(sin(st[0])*43758.5453123);
  float temp1 = fract(sin(st[1])*43758.5453123);
  return Eigen::Vector2d(temp0, temp1);
}
// Inputs:
//   st  3D seed
// Returns 2D random point in [0,1]²
Eigen::Vector2d random2(Eigen::Vector3d st){
  Eigen::Vector2d S = Eigen::Vector2d(st.dot(Eigen::Vector3d(127.1,311.7,783.089)),
             st.dot(Eigen::Vector3d(269.5,183.3,173.542)));
  float temp0 = fract(sin(S[0])*43758.5453123);
  float temp1 = fract(sin(S[1])*43758.5453123);
  return Eigen::Vector2d(temp0, temp1);
}

// Generate a pseudorandom unit 3D vector
// 
// Inputs:
//   seed  3D seed
// Returns psuedorandom, unit 3D vector drawn from uniform distribution over
// the unit sphere (assuming random2 is uniform over [0,1]²).
//
Eigen::Vector3d random_direction(Eigen::Vector3d seed)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code 
  Eigen::Vector2d random = random2(seed);
  float theta = 2*M_PI*random[0];
  float phi = M_PI*random[1];
  return Eigen::Vector3d(cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi)).normalized();
  /////////////////////////////////////////////////////////////////////////////
}
