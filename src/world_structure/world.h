#ifndef WORLD_STRUCTURE_WORLD_H
#define WORLD_STRUCTURE_WORLD_H

#include "world_structure/object.h"
#include <map>
#include <mutex>

#define object_id long

class world
{
public:
  world();
  object_id add_object(object o);
  object_id remove_object(object_id i);
  object get_object(object_id i);
private:
  object_id counter;
  std::map<object_id, object> list;
  std::mutex m;
};

#endif // WORLD_STRUCTURE_WORLD_H
