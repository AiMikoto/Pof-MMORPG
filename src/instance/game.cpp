#include "instance/game.h"
#include "instance/client.h"
#include "lib/log.h"

user_card_library ucl;
user_card_library uclp;

bool loaded = false;

void ucl_destroy()
{
}

void unload()
{
  BOOST_LOG_TRIVIAL(trace) << "unloading instance";
  loaded = false;
}

void load()
{
  BOOST_LOG_TRIVIAL(trace) << "loading instanced";
  loaded = true;
}

bool is_loaded()
{
  return loaded;
}
