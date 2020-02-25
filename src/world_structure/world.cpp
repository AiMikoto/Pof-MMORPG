#include "world_structure/world.h"
#include <map>
#include <mutex>

#define object_id long

world::world()
{
  this -> counter = 1;
}

object_id world::add_object(object o)
{
  this -> m.lock();
  this -> counter++;
  list[this -> counter] = o;
  return this -> counter;
  this -> m.unlock();
}

object_id world::remove_object(object_id i)
{
  this -> m.lock();
  auto it = list.find(i);
  if(it == list.end())
  {
    return 0;
  }
  list.erase(it);
  return i;
  this -> m.unlock();
}

object world::get_object(object_id i)
{
  this -> m.lock();
  return list[i];
  this -> m.unlock();
}
