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
  boost::chrono::system_clock::time_point last = boost::chrono::system_clock::now();
  forever
  {
    boost::chrono::duration<double> duration = last - boost::chrono::system_clock::now() + boost::chrono::milliseconds(10);
    boost::this_thread::sleep_for(boost::chrono::duration_cast<boost::chrono::microseconds>(duration));
    last = boost::chrono::system_clock::now();
    tick(current);
  }
}

