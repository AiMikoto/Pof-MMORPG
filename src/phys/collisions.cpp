#include "phys/collisions.h"

bool is_zero(glm::dvec3 v)
{
  return (std::abs(v.x) < 0.01) && (std::abs(v.y) < 0.01) && (std::abs(v.z) < 0.01);
}

bool box_box(container *b1, container *b2)
{
  int i, j;
  bool collides = true;
  glm::dvec4 points1[8], points2[8];
  // get points from b1
  get_points(b1, points1);
  // compute 3 edges for b1
  glm::dvec3 eb11 = points1[0] - points1[1];
  glm::dvec3 eb12 = points1[0] - points1[2];
  glm::dvec3 eb13 = points1[0] - points1[4];
  // get points from b2
  get_points(b2, points2);
  // compute 3 edges for b2
  glm::dvec3 eb21 = points2[0] - points2[1];
  glm::dvec3 eb22 = points2[0] - points2[2];
  glm::dvec3 eb23 = points2[0] - points2[4];
  // compute 15 axis
  glm::dvec3 axis[15] = {
    // 3 normals from b1
    glm::cross(eb11, eb12),
    glm::cross(eb11, eb13),
    glm::cross(eb12, eb13),
    // 3 normals from b2
    glm::cross(eb21, eb22),
    glm::cross(eb21, eb23),
    glm::cross(eb22, eb23),
    // 9 from crossproduct of b1 and b2 edges
    glm::cross(eb11, eb21),
    glm::cross(eb11, eb22),
    glm::cross(eb11, eb23),
    glm::cross(eb12, eb21),
    glm::cross(eb12, eb22),
    glm::cross(eb12, eb23),
    glm::cross(eb13, eb21),
    glm::cross(eb13, eb22),
    glm::cross(eb13, eb23)
  };
  double p;
  // for each axis
  for(i = 0; i < 15; i++)
  {
    if(is_zero(axis[i]))
    { // pointless
      continue;
    }
    axis[i] = glm::normalize(axis[i]);
    double min1p, max1p, min2p, max2p;
    // project b1 onto axis
    for(j = 0; j < 8; j++)
    {
      p = axis[i].x * points1[j].x
        + axis[i].y * points1[j].y
        + axis[i].z * points1[j].z;
      if(j == 0)
      {
        min1p = p;
        max1p = p;
      }
      else
      {
        min1p = std::min(min1p, p);
        max1p = std::max(max1p, p);
      }
    }
    // project b2 onto axis
    for(j = 0; j < 8; j++)
    {
      p = axis[i].x * points2[j].x
        + axis[i].y * points2[j].y
        + axis[i].z * points2[j].z;
      if(j == 0)
      {
        min2p = p;
        max2p = p;
      }
      else
      {
        min2p = std::min(min2p, p);
        max2p = std::max(max2p, p);
      }
    }
    if((max2p > min1p) && (max1p > min2p))
    {
      // collides on this axis
    }
    else
    {
      // no collision on this axis
      collides = false;
      break;
    }
  }
  return collides;
}
