#include "phys/collisions.h"

bool is_zero(glm::dvec3 v)
{
  return (std::abs(v.x) < 0.01) && (std::abs(v.y) < 0.01) && (std::abs(v.z) < 0.01);
}

bool box_box(container *b1, container *b2)
{
  glm::dvec3 axis;
  double projection;
  return box_box(b1, b2, &axis, &projection);
}

bool box_box(container *b1, container *b2, glm::dvec3 *axis, double *projection)
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
  glm::dvec3 axi[15] = {
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
    if(is_zero(axi[i]))
    { // pointless
      continue;
    }
    axi[i] = glm::normalize(axi[i]);
    double min1p, max1p, min2p, max2p;
    // project b1 onto axis
    for(j = 0; j < 8; j++)
    {
      p = axi[i].x * points1[j].x
        + axi[i].y * points1[j].y
        + axi[i].z * points1[j].z;
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
      p = axi[i].x * points2[j].x
        + axi[i].y * points2[j].y
        + axi[i].z * points2[j].z;
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
      double projection_difference = max2p - min1p;
      if(i == 0)
      {
        *projection = projection_difference;
        *axis = axi[i];
      }
      if(*projection > projection_difference)
      {
        *projection = projection_difference;
        *axis = axi[i];
      }
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


bool capsule_box(container *c, container *b)
{
  glm::dvec3 axis;
  double projection;
  return capsule_box(c, b, &axis, &projection);
}

bool capsule_box(container *c, container *b, glm::dvec3 *axis, double *projection)
{
  // TODO: this
}

bool box_capsule(container *b, container *c)
{
  return capsule_box(c, b);
}

bool box_capsule(container *b, container *c, glm::dvec3 *axis, double *projection)
{
  bool ret = capsule_box(c, b, axis, projection);
  *axis = -*axis;
  return ret;
}

bool capsule_capsule(container *c1, container *c2)
{
  glm::dvec3 axis;
  double projection;
  return capsule_capsule(c1, c2, &axis, &projection);
}

bool capsule_capsule(container *c1, container *c2, glm::dvec3 *axis, double *projection)
{
  // Assumptions
  // scale - x = z = radius, y = height
  // rotation - only around y axis -> no impact on aabb
  double r1 = c1 -> o -> transform.scale.x * c1 -> o -> meshScale.x / 2;
  double r2 = c2 -> o -> transform.scale.x * c2 -> o -> meshScale.x / 2;
  double miny1 = c1 -> o -> transform.position.y - c1 -> o -> transform.scale.y * c1 -> o -> meshScale.y / 2 + r1;
  double maxy1 = c1 -> o -> transform.position.y + c1 -> o -> transform.scale.y * c1 -> o -> meshScale.y / 2 - r1;
  double miny2 = c2 -> o -> transform.position.y - c2 -> o -> transform.scale.y * c2 -> o -> meshScale.y / 2 + r2;
  double maxy2 = c2 -> o -> transform.position.y + c2 -> o -> transform.scale.y * c2 -> o -> meshScale.y / 2 - r2;
  double dx = c1 -> o -> transform.position.x - c2 -> o -> transform.position.x;
  double dz = c1 -> o -> transform.position.z - c2 -> o -> transform.position.z;
  if((maxy2 > miny1) && (maxy1 > miny2))
  {
    // Just radius check
    double distance = std::sqrt(dx * dx + dz * dz);
    if(distance < r1 + r2)
    {
      *projection = r1 + r2 - distance;
      *axis = {dx, 0, dz};
      *axis = glm::normalize(*axis);
      return true;
    }
    return false;
  }
  if(maxy2 < miny1)
  {
    double dy = miny1 - maxy2;
    double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if(distance < r1 + r2)
    {
      *projection = r1 + r2 - distance;
      *axis = {dx, -dy, dz};
      *axis = glm::normalize(*axis);
      return true;
    }
    return false;
  }
  if(maxy1 < miny2)
  {
    double dy = miny2 - maxy1;
    double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if(distance < r1 + r2)
    {
      *projection = r1 + r2 - distance;
      *axis = {dx, dy, dz};
      *axis = glm::normalize(*axis);
      return true;
    }
    return false;
  }
  return false;
}
