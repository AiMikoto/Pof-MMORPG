#include "phys/environment.h"

environment::environment()
{
  counter = 0;
}

environment::~environment()
{
  // TODO: free capsules
}

int environment::add(container *c)
{
  counter;
  containers[counter] = c;
  return counter++;
}
