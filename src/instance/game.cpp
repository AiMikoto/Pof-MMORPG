#include "instance/game.h"
#include "instance/client.h"
#include "lib/log.h"
#include "include/common_macro.h"
#include "instance/misc.h"
#include "instance/ioc.h"
#include "lib/uuid.h"

user_card_library ucl;
user_card_library uclp;

engine::Scene *current;

bool loaded = false;

boost::thread slicer_t;

void ucl_destroy()
{
}

void unload()
{
  BOOST_LOG_TRIVIAL(trace) << "unloading instance";
  if(is_loaded())
  {
    loaded = false;
    slicer_t.join();
    BOOST_LOG_TRIVIAL(trace) << "deleting scene";
    int status;
    // TODO: check if maps needs to be saved
    if(false)
    {
      save(current -> map);
    }
    delete current;
  }
}

void save()
{
  save(current -> map);
}

void save(map_t map)
{
  slicer_acquire();
  db -> save_map(map, current);
  slicer_release();
}

void load(map_t map)
{
  unload();
  BOOST_LOG_TRIVIAL(trace) << "loading instance";
  int status;
  current = db -> load_map(map, &status);
  current -> tag = get_uuid();
  slicer_t = boost::thread(slicer);
  loaded = true;
}

bool is_loaded()
{
  return loaded;
}

unsigned long long game_inject_object()
{
  engine::GameObject *go = new engine::GameObject();
  slicer_acquire();
  unsigned long long pos = current -> addGameObject(go);
  slicer_inject_object(go);
  slicer_release();
  return pos;
}

void slicer()
{
  boost::chrono::system_clock::time_point last = boost::chrono::system_clock::now();
  forever_until(!loaded)
  {
    boost::chrono::duration<double> duration = last - boost::chrono::system_clock::now() + boost::chrono::milliseconds(1000 / (int)SPS);
    boost::this_thread::sleep_for(boost::chrono::duration_cast<boost::chrono::microseconds>(duration));
    last = boost::chrono::system_clock::now();
    BOOST_LOG_TRIVIAL(trace) << "computing new scene";
    slice_t next_slice = slice(current);
    apply_slice(current, next_slice);
    call c;
    c.tree().put(OPCODE, OP_SLICE);
    c.tree().put_child("data", next_slice.encode());
    ucl.apply(boost::bind(ucl_broadcast, _1, c));
  }
}

