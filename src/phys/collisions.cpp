#include "phys/collisions.h"
#include "lib/log.h"

void rotate_axis(glm::dvec3 *axis, glm::dvec3 p1, glm::dvec3 p2)
{
  double proj1 = axis -> x * p1.x
            + axis -> y * p1.y
            + axis -> z * p1.z;
  double proj2 = axis -> x * p2.x
            + axis -> y * p2.y
            + axis -> z * p2.z;
  if(proj2 > proj1)
  {
    *axis *= -1;
  }
}

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
{ // use SAT
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
      double projection_difference = std::min(max2p - min1p, max1p - min2p);
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
  rotate_axis(axis, b1 -> o -> transform.position, b2 -> o -> transform.position);
  return collides;
}


bool capsule_box(container *c, container *b)
{
  glm::dvec3 axis;
  double projection;
  return capsule_box(c, b, &axis, &projection);
}

bool capsule_box(container *c, container *b, glm::dvec3 *axis, double *projection)
{ // use SAT
  int i, j;
  bool collides = true;
  glm::dvec4 points[8];
  // get details of capsule, namely the radius and the 2 segment delimiters
  double r = c -> o -> transform.scale.x * c -> o -> meshScale.x / 2;
  double minyc = c -> o -> transform.position.y - c -> o -> transform.scale.y * c -> o -> meshScale.y / 2 + r;
  double maxyc = c -> o -> transform.position.y + c -> o -> transform.scale.y * c -> o -> meshScale.y / 2 - r;
  glm::dvec3 pointc1 = {c -> o -> transform.position.x, minyc, c -> o -> transform.position.z};
  glm::dvec3 pointc2 = {c -> o -> transform.position.x, maxyc, c -> o -> transform.position.z};
  // get points from b
  get_points(b, points);
  // compute 3 edges for b
  glm::dvec3 eb1 = points[0] - points[1];
  glm::dvec3 eb2 = points[0] - points[2];
  glm::dvec3 eb3 = points[0] - points[4];
  glm::dvec3 ec = pointc2 - pointc1;
  // compute a ton of axis
  glm::dvec3 axi[15] = {
    // 3 normals from b
    glm::cross(eb1, eb2),
    glm::cross(eb1, eb3),
    glm::cross(eb2, eb3),
    // 3 cross products from edges of b and the capsule segment
    glm::cross(eb1, ec),
    glm::cross(eb2, ec),
    glm::cross(eb2, ec),
    {points[0].x - pointc1.x, 0, points[0].z - pointc1.z},
    {points[1].x - pointc1.x, 0, points[1].z - pointc1.z},
    {points[2].x - pointc1.x, 0, points[2].z - pointc1.z},
    {points[3].x - pointc1.x, 0, points[3].z - pointc1.z},
    {points[4].x - pointc1.x, 0, points[4].z - pointc1.z},
    {points[5].x - pointc1.x, 0, points[5].z - pointc1.z},
    {points[6].x - pointc1.x, 0, points[6].z - pointc1.z},
    {points[7].x - pointc1.x, 0, points[7].z - pointc1.z},
    ec
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
    // project b onto axis
    for(j = 0; j < 8; j++)
    {
      p = axi[i].x * points[j].x
        + axi[i].y * points[j].y
        + axi[i].z * points[j].z;
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
    // project the capsule onto axis
    p = axi[i].x * pointc1.x
      + axi[i].y * pointc1.y
      + axi[i].z * pointc1.z;
    min2p = p;
    max2p = p;
    p = axi[i].x * pointc2.x
      + axi[i].y * pointc2.y
      + axi[i].z * pointc2.z;
    min2p = std::min(min2p, p);
    max2p = std::max(max2p, p);
    // project the radius shadow onto axis
    min2p -= r;
    max2p += r;
    if((max2p > min1p) && (max1p > min2p))
    {
      // collides on this axis
      double projection_difference = std::min(max2p - min1p, max1p - min2p);
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
  rotate_axis(axis, c -> o -> transform.position, b -> o -> transform.position);
  return collides;
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
      if(is_zero(*axis))
      {
        *axis = {0, 1, 0};
      }
      *axis = glm::normalize(*axis);
      rotate_axis(axis, c1 -> o -> transform.position, c2 -> o -> transform.position);
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
      if(is_zero(*axis))
      {
        *axis = {0, 1, 0};
      }
      *axis = glm::normalize(*axis);
      rotate_axis(axis, c1 -> o -> transform.position, c2 -> o -> transform.position);
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
      if(is_zero(*axis))
      {
        *axis = {0, 1, 0};
      }
      *axis = glm::normalize(*axis);
      rotate_axis(axis, c1 -> o -> transform.position, c2 -> o -> transform.position);
      return true;
    }
    return false;
  }
  return false;
}

bool collide(container *c1, container *c2)
{
  glm::dvec3 axis;
  double projection;
  return collide(c1, c2, &axis, &projection);
}

bool collide(container *c1, container *c2, glm::dvec3 *axis, double *projection)
{
  if((c1 -> type == box) && (c2 -> type == box))
  {
    BOOST_LOG_TRIVIAL(trace) << "box to box collision";
    return box_box(c1, c2, axis, projection);
  }
  if((c1 -> type == box) && (c2 -> type == caps))
  {
    BOOST_LOG_TRIVIAL(trace) << "box to capsule collision";
    return box_capsule(c1, c2, axis, projection);
  }
  if((c1 -> type == caps) && (c2 -> type == box))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to box collision";
    return capsule_box(c1, c2, axis, projection);
  }
  if((c1 -> type == caps) && (c2 -> type == caps))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to capsule collision";
    return capsule_capsule(c1, c2, axis, projection);
  }
  BOOST_LOG_TRIVIAL(error) << "unknown collision type";
  return false;
}
