#include "phys/environment.h"

environment::environment()
{
  counter = 0;
}

environment::~environment()
{
  // TODO: free capsules
}

int environment::add(capsule *c)
{
  counter;
  capsules[counter] = c;
  return counter++;
}
