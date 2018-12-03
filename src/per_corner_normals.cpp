#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>
#include <cmath>

#define PI 3.14159265358979323846

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(F.rows()*3,3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  std::vector<std::vector<int>> VF;
  vertex_triangle_adjacency(F, V.rows(), VF);
  Eigen::MatrixXd facenormals;
  facenormals.resize(F.rows(), 3);
  for (int i = 0; i < F.rows(); i++) {
    facenormals.row(i) << triangle_area_normal(V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2))).normalized();
  }

  for (int i = 0; i < F.rows(); i++) {
    Eigen::RowVector3d first_normal = facenormals.row(i);
    for (int j = 0; j < 3; j++) {
      Eigen::RowVector3d curr_sum(0, 0, 0);
      int ind_vtx = F(i, j);
      for (int k = 0; k < VF[ind_vtx].size(); k++) {
        Eigen::RowVector3d curr_normal = facenormals.row(VF[ind_vtx][k]);
        if ((curr_normal.dot(first_normal)/(curr_normal.norm()*first_normal.norm())) >= cos(corner_threshold*PI/180.0)) {
          curr_sum += curr_normal;
        }
      }
      N.row(3*i + j) << curr_sum.normalized();
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
