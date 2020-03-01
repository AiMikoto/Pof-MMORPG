#include "lib/call.h"
#include "lib/log.h"
#include "client/ioc.h"
#include "client/client.h"
#include <boost/thread/thread.hpp>
#include "client/game.h"
#include "include/maps.h"
#include "include/regions.h"

#define LOGIN_SV_HOST "localhost"
#define LOGIN_SV_PORT 7777

boost::asio::io_context ioc;
instance *current_instance;
std::mutex init_l;
std::string username = "joe";

int main(int argc, char **argv)
{
  log_init("client");
  BOOST_LOG_TRIVIAL(trace) << "client initialising";
  init_l.lock();
  // artificially create a new game
  BOOST_LOG_TRIVIAL(trace) << "attempting to connect to login server";
  current_instance = instance_builder(LOGIN_SV_HOST, LOGIN_SV_PORT);
  BOOST_LOG_TRIVIAL(trace) << "attempting to login";
  current_instance -> authenticate(username, "p455w0rd");
  while(!ucl.contains(username))
  {
    BOOST_LOG_TRIVIAL(trace) << "waiting for user card";
    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }
  BOOST_LOG_TRIVIAL(trace) << "client finished initialisation";
  init_l.unlock();
  std::mutex m;
  boost::this_thread::sleep(boost::posix_time::seconds(15));
  BOOST_LOG_TRIVIAL(trace) << "client changing map ARTIFICIALLY";
  current_instance -> change_map(MAP_FLATLANDS, REG_EU);
  m.lock();
  BOOST_LOG_TRIVIAL(error) << "client finished successfully";
  m.lock();
  return 0;
}
