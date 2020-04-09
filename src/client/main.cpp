#include "lib/call.h"
#include "lib/log.h"
#include "client/ioc.h"
#include "client/client.h"
#include <boost/thread/thread.hpp>
#include "client/game.h"
#include "include/maps.h"
#include "include/regions.h"
#include "client/graphics.h"
#include <boost/thread/barrier.hpp>
#include "client/shutdown.h"
#include "client/crypto.h"

#ifdef __linux__
#include <csignal>
#endif

#define LOGIN_SV_HOST "localhost"
#define LOGIN_SV_PORT 7777

instance *current_instance;
std::mutex init_l;
std::string username;

int main(int argc, char **argv)
{
  log_init("client");
  std::string pub = "keys/public_key.pem";
  std::string host = LOGIN_SV_HOST;
  int port = LOGIN_SV_PORT;
  std::string password = "";
  username = "";
  // parsing arguments;
  std::string *args = new std::string[argc];
  for(int i = 0; i < argc; i++)
  {
    args[i] = std::string(argv[i]);
  }
  for(int i = 1; i < argc; i++)
  {
    if(args[i] == "-pub")
    {
      pub = args[++i];
      continue;
    }
    if(args[i] == "-login_server")
    {
      host = args[++i];
      port = std::stoi(args[++i]);
      continue;
    }
    if(args[i] == "-login_details")
    {
      username = args[++i];
      password = args[++i];
      continue;
    }
    BOOST_LOG_TRIVIAL(warning) << "unknown parameter " << args[i];
  }
#ifdef __linux__
  BOOST_LOG_TRIVIAL(trace) << "loading handler for SIGINT";
  std::signal(SIGINT, shutdown);
#endif
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub);
  BOOST_LOG_TRIVIAL(trace) << "client initialising";
  init_l.lock();
  BOOST_LOG_TRIVIAL(trace) << "initialising graphics";
  gfx_init();
  BOOST_LOG_TRIVIAL(trace) << "initialising game";
  game_init();
  // artificially create a new game
  BOOST_LOG_TRIVIAL(trace) << "attempting to connect to login server";
  current_instance = instance_builder(host, port);
  BOOST_LOG_TRIVIAL(trace) << "attempting to login";
  if(!current_instance -> authenticate(username, password))
  {
    BOOST_LOG_TRIVIAL(error) << "authentication refused by login server.";
    goto cleanup;
  }
  while(!ucl.contains(username))
  {
    BOOST_LOG_TRIVIAL(trace) << "waiting for user card";
    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }
  BOOST_LOG_TRIVIAL(trace) << "client finished initialisation";
  init_l.unlock();
  main_barrier.wait();
cleanup:
  BOOST_LOG_TRIVIAL(error) << "shutdown initiated";
  BOOST_LOG_TRIVIAL(error) << "deleting connection";
  delete current_instance;
  BOOST_LOG_TRIVIAL(trace) << "destroying game";
  game_destroy();
  BOOST_LOG_TRIVIAL(error) << "destroying gfx";
  gfx_destroy();
  BOOST_LOG_TRIVIAL(error) << "deleting keys";
  destroy_crypto();
  return 0;
}
