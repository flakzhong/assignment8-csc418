#include "insert_box_into_box.h"

void insert_box_into_box(
  const BoundingBox & A,
  BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  Eigen::RowVector3d a_min = A.min_corner;
  Eigen::RowVector3d a_max = A.max_corner;
  Eigen::RowVector3d b_min = B.min_corner;
  Eigen::RowVector3d b_max = B.max_corner;
  for (int i = 0; i < 3; i++) {
    B.min_corner[i] = (b_min[i] < a_min[i])?b_min[i]:a_min[i];
    B.max_corner[i] = (b_max[i] < a_max[i])?a_max[i]:b_max[i];
  }
  ////////////////////////////////////////////////////////////////////////////
}

