#include "lib/call.h"
#include "lib/log.h"
#include "client/ioc.h"
#include "client/client.h"
#include <boost/thread/thread.hpp>
#include "client/game.h"
#include "include/maps.h"
#include "include/regions.h"
#include <boost/thread/barrier.hpp>

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
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub);
  BOOST_LOG_TRIVIAL(trace) << "client initialising";
  init_l.lock();
  // artificially create a new game
  BOOST_LOG_TRIVIAL(trace) << "attempting to connect to login server";
  current_instance = instance_builder(host, port);
  BOOST_LOG_TRIVIAL(trace) << "attempting to login";
  if(!current_instance -> authenticate(username, password))
  {
    BOOST_LOG_TRIVIAL(error) << "authentication refused by login server.";
  }
  while(!ucl.contains(username))
  {
    BOOST_LOG_TRIVIAL(trace) << "waiting for user card";
    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }
  BOOST_LOG_TRIVIAL(trace) << "client finished initialisation";
  init_l.unlock();
  boost::this_thread::sleep(boost::posix_time::seconds(25));
  BOOST_LOG_TRIVIAL(trace) << "client changing map ARTIFICIALLY";
  current_instance -> change_map(MAP_FLATLANDS, REG_EU);
  boost::this_thread::sleep(boost::posix_time::seconds(25));
  send_message(world, "fluffy kittens");
  boost::barrier b(2);
  BOOST_LOG_TRIVIAL(error) << "client finished successfully";
  b.wait();
  return 0;
}
