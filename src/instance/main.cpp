#include "lib/log.h"
#include "instance/server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include "instance/ioc.h"
#include "instance/crypto.h"
#include <boost/thread/barrier.hpp>
#include "instance/shutdown.h"
#include "instance/chat_client.h"
#include "instance/game.h"
#include "instance/misc.h"
#include "instance/token.h"

#include "components/phys_collider.h"
#include "components/solid_object.h"
#include "components/meshFilter.h"
#include "components/meshRenderer.h"

#ifdef __linux__
#include <csignal>
#endif

std::string my_token = "fish";

database *db;

int main(int argc, char **argv)
{
  std::string pub = "keys/public_key.pem";
  std::string pri = "keys/private_key.pem";
  int port = 7000;
  // parsing arguments;
  std::string *args = new std::string[argc];
  for(int i = 0; i < argc; i++)
  {
    args[i] = std::string(argv[i]);
  }
  for(int i = 1; i < argc; i++)
  {
    if(args[i] == "-tok")
    {
      my_token = args[++i];
      continue;
    }
    if(args[i] == "-pub")
    {
      pub = args[++i];
      continue;
    }
    if(args[i] == "-priv")
    {
      pri = args[++i];
      continue;
    }
    if(args[i] == "-port")
    {
      port = std::stoi(args[++i]);
      continue;
    }
    BOOST_LOG_TRIVIAL(warning) << "unknown parameter " << args[i];
  }
  log_init("instance");
#ifdef __linux__
  BOOST_LOG_TRIVIAL(trace) << "loading handler for SIGINT";
  std::signal(SIGINT, shutdown);
#endif
  BOOST_LOG_TRIVIAL(trace) << "initialising database";
  db = db_init();
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub, pri);
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server *s = new server(port);
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  main_barrier.wait();
  BOOST_LOG_TRIVIAL(trace) << "cleaning up server";
  delete s;
  if(chat)
  {
    BOOST_LOG_TRIVIAL(trace) << "cleaning up chat client";
    delete chat;
  }
  BOOST_LOG_TRIVIAL(trace) << "saving users";
  ucl.apply(saver);
  BOOST_LOG_TRIVIAL(trace) << "saving pending users";
  uclp.apply(saver);
  BOOST_LOG_TRIVIAL(trace) << "destroying user card libraries";
  ucl_destroy();
  if(is_loaded())
  {
    BOOST_LOG_TRIVIAL(trace) << "unloading instance";
    unload();
  }
  BOOST_LOG_TRIVIAL(trace) << "destroying keys";
  destroy_crypto();
  BOOST_LOG_TRIVIAL(trace) << "closing database";
  delete db;
  BOOST_LOG_TRIVIAL(trace) << "deleting argument array";
  delete[] args;
  return 0;
}
