#ifndef PHYS_COLLISIONS_H
#define PHYS_COLLISIONS_H

#include "phys/container.h"

bool box_box(container *b1, container *b2);

bool box_box(container *b1, container *b2, glm::dvec3 *axis, double *projection);

bool capsule_box(container *c, container *b);

bool capsule_box(container *c, container *b, glm::dvec3 *axis, double *projection);

bool box_capsule(container *b, container *c);

bool box_capsule(container *b, container *c, glm::dvec3 *axis, double *projection);

bool capsule_capsule(container *c1, container *c2);

bool capsule_capsule(container *c1, container *c2, glm::dvec3 *axis, double *projection);

#endif // PHYS_COLLISIONS_H
