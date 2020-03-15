#ifndef PHYS_COLLISIONS_H
#define PHYS_COLLISIONS_H

#include "phys/container.h"

bool box_box(container *b1, container *b2);

bool box_box(container *b1, container *b2, glm::dvec3 *axis, double *projection);

#endif // PHYS_COLLISIONS_H
