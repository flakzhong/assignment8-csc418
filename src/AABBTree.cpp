#include "AABBTree.h"
#include "insert_box_into_box.h"
#include "first_hit.h"

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  for (int i = 0; i < objects.size(); i++) {
    insert_box_into_box(objects[i]->box, this->box);
  }
  if (objects.size() == 2) {
    this->left = objects[0];
    this->right = objects[1];
  } else {
    int axis = 0;
    for (int i = 0; i < 3; i++) {
      if (this->box.max_corner[i] - this->box.min_corner[i]
        >= this->box.max_corner[axis] - this->box.min_corner[axis]) {
          axis = i;
      }
    }
    double axis_center = (this->box.max_corner[axis] + this->box.min_corner[axis])/2;
    std::vector<std::shared_ptr<Object>> left, right;
    for (int i = 0; i < objects.size(); i++) {
      if ((objects[i]->box).center()[axis] < axis_center) {
        left.push_back(objects[i]);
      } else {
        right.push_back(objects[i]);
      }
    }

    if (left.size() == 0) {
      this->left = right[right.size() - 1];
      right.pop_back();
    } else if (right.size() == 0) {
      this->right = left[left.size() - 1];
      left.pop_back();
    }

    if (left.size() == 1) {
      this->left = left[0];
    } else if (left.size() > 1){
      this->left = std::make_shared<AABBTree>(left, depth+1);
    }
    if (right.size() == 1) {
      this->right = right[0];
    } else if (right.size() > 1) {
      this->right = std::make_shared<AABBTree>(right, depth+1);
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}

bool AABBTree::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  double temp_t;
  if (!ray_intersect_box(ray, this->box, min_t, temp_t)) {
    return false;
  } else {
    double tl, tr;
    Eigen::Vector3d l_n(-1, -1, -1);
    bool hl = this->left->intersect(ray, min_t, tl, l_n);

    Eigen::Vector3d r_n(-1, -1, -1);
    bool hr = this->right->intersect(ray, min_t, tr, r_n);
    if (hl && hr) {
      if (tl < tr) {
        t = tl;
        n = l_n;
      } else {
        t = tr;
        n = r_n;
      }
      return true;
    } else if (hl) {
      t = tl;
      n = l_n;
      return true;
    } else if (hr) {
      t = tr;
      n = r_n;
      return true;
    }
  }
  return false;
  ////////////////////////////////////////////////////////////////////////////
}
