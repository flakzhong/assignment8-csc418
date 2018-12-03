#ifndef AABBTREE_H
#define AABBTREE_H

#include "BoundingBox.h"
#include "ray_intersect_box.h"
#include "Object.h"
#include <Eigen/Core>
#include <memory>
#include <vector>

class AABBTree : public Object
{
  public:
    // // A soup is just a set (list) of triangles
    // std::vector<std::shared_ptr<Object> > triangles;


    // Pointers to left and right subtree branches. These could be another
    // AABBTree (internal node) or a leaf (primitive Object like MeshTriangle, or
    // CloudPoint)
    std::shared_ptr<Object> left;
    std::shared_ptr<Object> right;
    // For debugging, keep track of the depth (root has depth == 0)
    int depth;
    // For debugging, keep track of the number leaf, descendants 
    int num_leaves;
    // Construct a axis-aligned bounding box tree given a list of objects. Use the
    // midpoint along the longest axis of the box containing the given objects to
    // determine the left-right split.
    //
    // Inputs:
    //   objects  list of objects to store in this AABBTree
    //   Optional inputs:
    //     depth  depth of this tree (usually set by constructor of parent as
    //       their depth+1)
    // Side effects: num_leaves is set to objects.size() and left/right pointers
    // set to subtrees or leaf Objects accordingly.
    AABBTree(
      const std::vector<std::shared_ptr<Object> > & objects, 
      int depth=0);

    // Intersect a triangle soup with ray.
    //
    // Inputs:
    //   Ray  ray to intersect with
    //   min_t  minimum parametric distance to consider
    // Outputs:
    //   t  first intersection at ray.origin + t * ray.direction
    //   n  surface normal at point of intersection
    // Returns iff there a first intersection is found.
    bool intersect(
      const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const;

};

#endif

