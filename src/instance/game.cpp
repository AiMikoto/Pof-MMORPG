#include "instance/game.h"
#include "instance/client.h"
#include "lib/log.h"
#include "include/common_macro.h"

user_card_library ucl;
user_card_library uclp;

engine::Scene *current;

bool loaded = false;

void ucl_destroy()
{
}

void unload()
{
  BOOST_LOG_TRIVIAL(trace) << "unloading instance";
  if(is_loaded())
  {
    loaded = false;
    // TODO: wait for slicing thread to finish current slice
    BOOST_LOG_TRIVIAL(trace) << "deleting scene";
    // TODO: save scene
    delete current;
  }
}

void load()
{
  unload();
  BOOST_LOG_TRIVIAL(trace) << "loading instanced";
  // TODO: load scene
  current = new engine::Scene();
  // TODO: create slicing thread
  loaded = true;
}

bool is_loaded()
{
  return loaded;
}

void slicer()
{
  // TODO: update last timestamp
  forever
  {
    // TODO: sleep until next tick
    // TODO: update last timestamp
    tick(current);
  }
}

