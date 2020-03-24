#ifndef PHYS_SLICING_H
#define PHYS_SLICING_H

#include "scene/scene.h"

extern const double sps;
extern const double BAUMGARDE_CONSTANT;
extern const double COEFFICIENT_OF_RESTITUTION;

engine::Scene *tick(engine::Scene *e);

#endif // PHYS_SLICING_H
