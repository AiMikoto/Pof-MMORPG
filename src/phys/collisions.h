#ifndef PHYS_COLLISIONS_H
#define PHYS_COLLISIONS_H

#include "components/collider.h"

bool box_box(collider *b1, collider *b2);

bool box_box(collider *b1, collider *b2, glm::dvec3 *axis, double *projection);

bool capsule_box(collider *c, collider *b);

bool capsule_box(collider *c, collider *b, glm::dvec3 *axis, double *projection);

bool box_capsule(collider *b, collider *c);

bool box_capsule(collider *b, collider *c, glm::dvec3 *axis, double *projection);

bool capsule_capsule(collider *c1, collider *c2);

bool capsule_capsule(collider *c1, collider *c2, glm::dvec3 *axis, double *projection);

bool sphere_sphere(collider *s1, collider *s2);

bool sphere_sphere(collider *s1, collider *s2, glm::dvec3 *axis, double *projection);

bool sphere_box(collider *s, collider *b);

bool sphere_box(collider *s, collider *b, glm::dvec3 *axis, double *projection);

bool box_sphere(collider *b, collider *s);

bool box_sphere(collider *b, collider *s, glm::dvec3 *axis, double *projection);

bool collide(collider *c1, collider *c2);

bool collide(collider *c1, collider *c2, glm::dvec3 *axis, double *projection);

#endif // PHYS_COLLISIONS_H
