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

glm::dvec3 point_shortest_to_surface(glm::dvec3 point, glm::dvec3 sp1, glm::dvec3 sp2, glm::dvec3 sp3)
{
  // get the 2 lengths of the surface
  glm::dvec3 e1 = sp2 - sp1;
  glm::dvec3 e2 = sp3 - sp1;
  // generate 2 axis for the surface
  glm::dvec3 avec = glm::normalize(e1);
  glm::dvec3 bvec = glm::normalize(e2);
  // get the surface normal
  glm::dvec3 snor = glm::normalize(glm::cross(avec, bvec));
  // compute surface min and max delimiters
  double amin = 0;
  double bmin = 0;
  double amax = glm::dot(e1, avec);
  double bmax = glm::dot(e2, bvec);
  // project point onto surface plane
  glm::dvec3 pointp = point - glm::dot(snor, point) * snor - sp1;
  double ap = glm::dot(pointp, avec);
  double bp = glm::dot(pointp, bvec);
  // clamp projection's a vector component
  if(ap < amin)
  {
    ap = amin;
  }
  if(ap > amax)
  {
    ap = amax;
  }
  // clamp projection's b vector component
  if(bp < bmin)
  {
    bp = bmin;
  }
  if(bp > bmax)
  {
    bp = bmax;
  }
  // reconstruct closest point on the surface
  glm::dvec3 closest_point = sp1 + ap * avec + bp * bvec;
  return closest_point - point;
}

glm::dvec3 segment_shortest_to_surface(glm::dvec3 point1, glm::dvec3 point2, glm::dvec3 sp1, glm::dvec3 sp2, glm::dvec3 sp3)
{
  // get the 2 lengths of the surface
  glm::dvec3 e1 = sp2 - sp1;
  glm::dvec3 e2 = sp3 - sp1;
  glm::dvec3 seg = point2 - point1;
  // generate 2 axis for the surface
  glm::dvec3 avec = glm::normalize(e1);
  glm::dvec3 bvec = glm::normalize(e2);
  // compute segment axis;
  glm::dvec3 svec = glm::normalize(seg);
  // get the surface normal
  glm::dvec3 snor = glm::normalize(glm::cross(avec, bvec));
  // compute surface min and max delimiters
  double amin = 0;
  double bmin = 0;
  double amax = glm::dot(e1, avec);
  double bmax = glm::dot(e2, bvec);
  // compute segment min and max delimiters
  double smin = 0;
  double smax = glm::dot(seg, svec);
  // compute segment offset
  double spd = glm::dot(point1, svec);
  // project point 1 onto surface plane
  glm::dvec3 point1p = point1 - glm::dot(snor, point1) * snor - sp1;
  double a1p = glm::dot(point1p, avec);
  double b1p = glm::dot(point1p, bvec);
  // project point 2 onto surface plane
  glm::dvec3 point2p = point2 - glm::dot(snor, point2) * snor - sp1;
  double a2p = glm::dot(point2p, avec);
  double b2p = glm::dot(point2p, bvec);
  // clamp point 1 projection's a vector component
  if(a1p < amin)
  {
    a1p = amin;
  }
  if(a1p > amax)
  {
    a1p = amax;
  }
  // clamp point 1 projection's b vector component
  if(b1p < bmin)
  {
    b1p = bmin;
  }
  if(b1p > bmax)
  {
    b1p = bmax;
  }
  // clamp point 2 projection's a vector component
  if(a2p < amin)
  {
    a2p = amin;
  }
  if(a2p > amax)
  {
    a2p = amax;
  }
  // clamp point 2 projection's b vector component
  if(b2p < bmin)
  {
    b2p = bmin;
  }
  if(b2p > bmax)
  {
    b2p = bmax;
  }
  // project point 1 back onto the segment
  glm::dvec3 pp1 = sp1 + a1p * avec + b1p * bvec;
  double s1p = glm::dot(pp1, svec) - spd;
  // project point 2 back onto the segment
  glm::dvec3 pp2 = sp1 + a2p * avec + b2p * bvec;
  double s2p = glm::dot(pp2, svec) - spd;
  // clamp point 1 onto segment
  if(s1p < smin)
  {
    s1p = smin;
  }
  if(s1p > smax)
  {
    s1p = smax;
  }
  // clamp point 2 onto segment
  if(s2p < smin)
  {
    s2p = smin;
  }
  if(s2p > smax)
  {
    s2p = smax;
  }
  // compute distances and take minimum of those
  glm::dvec3 rp1 = point1 + s1p * svec;
  double d1 = glm::distance(pp1, rp1);
  glm::dvec3 rp2 = point1 + s2p * svec;
  double d2 = glm::distance(pp2, rp2);
  if(d1 < d2)
  {
    return pp1 - rp1;
  }
  return pp2 - rp2;
}

bool box_box(collider *b1, collider *b2)
{
  glm::dvec3 axis;
  double projection;
  return box_box(b1, b2, &axis, &projection);
}

bool box_box(collider *b1, collider *b2, glm::dvec3 *axis, double *projection)
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
  rotate_axis(axis, b1 -> gameObject -> transform.position, b2 -> gameObject -> transform.position);
  return collides;
}


bool capsule_box(collider *c, collider *b)
{
  glm::dvec3 axis;
  double projection;
  return capsule_box(c, b, &axis, &projection);
}

bool capsule_box(collider *c, collider *b, glm::dvec3 *axis, double *projection)
{ // use SAT
  int i, j;
  bool collides = true;
  glm::dvec4 points[8];
  // get details of capsule, namely the radius and the 2 segment delimiters
  double r = c -> gameObject -> transform.scale.x * c -> size.x / 2;
  double minyc = c -> gameObject -> transform.position.y - c -> gameObject -> transform.scale.y * c -> size.y / 2 + r;
  double maxyc = c -> gameObject -> transform.position.y + c -> gameObject -> transform.scale.y * c -> size.y / 2 - r;
  glm::dvec3 pointc1 = {c -> gameObject -> transform.position.x, minyc, c -> gameObject -> transform.position.z};
  glm::dvec3 pointc2 = {c -> gameObject -> transform.position.x, maxyc, c -> gameObject -> transform.position.z};
  // get points from b
  get_points(b, points);
  // compute 6 axis
  glm::dvec3 axi[6] = {
    // 6 from minimum distance from each box surface to capsule segment
    segment_shortest_to_surface(pointc1, pointc2, points[7], points[6], points[5]),
    segment_shortest_to_surface(pointc1, pointc2, points[7], points[5], points[3]),
    segment_shortest_to_surface(pointc1, pointc2, points[7], points[6], points[3]),
    segment_shortest_to_surface(pointc1, pointc2, points[5], points[4], points[1]),
    segment_shortest_to_surface(pointc1, pointc2, points[3], points[2], points[1]),
    segment_shortest_to_surface(pointc1, pointc2, points[6], points[4], points[2])
  };
  double p;
  // for each axis
  for(i = 0; i < 6; i++)
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
  rotate_axis(axis, c -> gameObject -> transform.position, b -> gameObject -> transform.position);
  return collides;
}

bool box_capsule(collider *b, collider *c)
{
  return capsule_box(c, b);
}

bool box_capsule(collider *b, collider *c, glm::dvec3 *axis, double *projection)
{
  bool ret = capsule_box(c, b, axis, projection);
  *axis = -*axis;
  return ret;
}

bool capsule_capsule(collider *c1, collider *c2)
{
  glm::dvec3 axis;
  double projection;
  return capsule_capsule(c1, c2, &axis, &projection);
}

bool capsule_capsule(collider *c1, collider *c2, glm::dvec3 *axis, double *projection)
{
  // Assumptions
  // scale - x = z = radius, y = height
  // rotation - only around y axis -> no impact on aabb
  double r1 = c1 -> gameObject -> transform.scale.x * c1 -> size.x / 2;
  double r2 = c2 -> gameObject -> transform.scale.x * c2 -> size.x / 2;
  double miny1 = c1 -> gameObject -> transform.position.y - c1 -> gameObject -> transform.scale.y * c1 -> size.y / 2 + r1;
  double maxy1 = c1 -> gameObject -> transform.position.y + c1 -> gameObject -> transform.scale.y * c1 -> size.y / 2 - r1;
  double miny2 = c2 -> gameObject -> transform.position.y - c2 -> gameObject -> transform.scale.y * c2 -> size.y / 2 + r2;
  double maxy2 = c2 -> gameObject -> transform.position.y + c2 -> gameObject -> transform.scale.y * c2 -> size.y / 2 - r2;
  double dx = c1 -> gameObject -> transform.position.x - c2 -> gameObject -> transform.position.x;
  double dz = c1 -> gameObject -> transform.position.z - c2 -> gameObject -> transform.position.z;
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
      rotate_axis(axis, c1 -> gameObject -> transform.position, c2 -> gameObject -> transform.position);
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
      rotate_axis(axis, c1 -> gameObject -> transform.position, c2 -> gameObject -> transform.position);
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
      rotate_axis(axis, c1 -> gameObject -> transform.position, c2 -> gameObject -> transform.position);
      return true;
    }
    return false;
  }
  return false;
}

bool sphere_sphere(collider *s1, collider *s2)
{
  glm::dvec3 axis;
  double projection;
  return sphere_sphere(s1, s2, &axis, &projection);
}

bool sphere_sphere(collider *s1, collider *s2, glm::dvec3 *axis, double *projection)
{
  glm::dvec3 point1 = s1 -> gameObject -> transform.position;
  glm::dvec3 point2 = s2 -> gameObject -> transform.position;
  *axis = point2 - point1;
  if(is_zero(*axis))
  {
    *axis = {0, 1, 0};
  }
  *axis = glm::normalize(*axis);
  rotate_axis(axis, point1, point2);
  double dx = s1 -> gameObject -> transform.position.x - s2 -> gameObject -> transform.position.x;
  double dy = s1 -> gameObject -> transform.position.y - s2 -> gameObject -> transform.position.y;
  double dz = s1 -> gameObject -> transform.position.z - s2 -> gameObject -> transform.position.z;
  double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
  double radius1 = s1 -> gameObject -> transform.scale.x * s1 -> size.x / 2;
  double radius2 = s2 -> gameObject -> transform.scale.x * s2 -> size.x / 2;
  if(distance >= radius1 + radius2)
  {
    return false;
  }
  else
  {
    *projection = radius1 + radius2 - distance;
    return true;
  }
}

bool sphere_box(collider *s, collider *b)
{
  return box_sphere(b, s);
}

bool sphere_box(collider *s, collider *b, glm::dvec3 *axis, double *projection)
{
  bool ret = box_sphere(b, s, axis, projection);
  *axis = -*axis;
  return ret;
}

bool box_sphere(collider *b, collider *s)
{
  glm::dvec3 axis;
  double projection;
  return capsule_capsule(b, s, &axis, &projection);
}

bool box_sphere(collider *b, collider *s, glm::dvec3 *axis, double *projection)
{ // use SAT
  int i, j;
  bool collides = true;
  glm::dvec4 points[8];
  // get sphere radius and center
  double r = s -> gameObject -> transform.scale.x * s -> size.x / 2;
  glm::dvec3 point = s -> gameObject -> transform.position;
  // get points from b
  get_points(b, points);
  // generate 6 axis
  glm::dvec3 axi[6] = {
    // 6 from minimum distance from each box surface to capsule segment
    point_shortest_to_surface(point, points[7], points[6], points[5]),
    point_shortest_to_surface(point, points[7], points[5], points[3]),
    point_shortest_to_surface(point, points[7], points[6], points[3]),
    point_shortest_to_surface(point, points[5], points[4], points[1]),
    point_shortest_to_surface(point, points[3], points[2], points[1]),
    point_shortest_to_surface(point, points[6], points[4], points[2])
  };
  double p;
  // for each axis
  for(i = 0; i < 6; i++)
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
    // project the sphere onto axis
    p = axi[i].x * point.x
      + axi[i].y * point.y
      + axi[i].z * point.z;
    min2p = p;
    max2p = p;
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
  rotate_axis(axis, s -> gameObject -> transform.position, b -> gameObject -> transform.position);
  return collides;
}

bool collide(collider *c1, collider *c2)
{
  glm::dvec3 axis;
  double projection;
  return collide(c1, c2, &axis, &projection);
}

bool collide(collider *c1, collider *c2, glm::dvec3 *axis, double *projection)
{
  if((c1 -> c_type == box) && (c2 -> c_type == box))
  {
    BOOST_LOG_TRIVIAL(trace) << "box to box collision";
    return box_box(c1, c2, axis, projection);
  }
  if((c1 -> c_type == box) && (c2 -> c_type == caps))
  {
    BOOST_LOG_TRIVIAL(trace) << "box to capsule collision";
    return box_capsule(c1, c2, axis, projection);
  }
  if((c1 -> c_type == caps) && (c2 -> c_type == box))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to box collision";
    return capsule_box(c1, c2, axis, projection);
  }
  if((c1 -> c_type == caps) && (c2 -> c_type == caps))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to capsule collision";
    return capsule_capsule(c1, c2, axis, projection);
  }
  // capsule is generalised sphere, so caps-sphere can be interpreted as capsule capsule
  if((c1 -> c_type == sphere) && (c2 -> c_type == caps))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to capsule collision";
    return capsule_capsule(c1, c2, axis, projection);
  }
  if((c1 -> c_type == caps) && (c2 -> c_type == sphere))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to capsule collision";
    return capsule_capsule(c1, c2, axis, projection);
  }
  if((c1 -> c_type == sphere) && (c2 -> c_type == sphere))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to capsule collision";
    return sphere_sphere(c1, c2, axis, projection);
  }
  if((c1 -> c_type == sphere) && (c2 -> c_type == box))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to capsule collision";
    return sphere_box(c1, c2, axis, projection);
  }
  if((c1 -> c_type == box) && (c2 -> c_type == sphere))
  {
    BOOST_LOG_TRIVIAL(trace) << "capsule to capsule collision";
    return box_sphere(c1, c2, axis, projection);
  }
  BOOST_LOG_TRIVIAL(error) << "unknown collision type";
  return false;
}
