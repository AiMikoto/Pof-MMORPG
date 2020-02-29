#include "lib/call.h"
#include "lib/log.h"
#include "client/ioc.h"
#include "client/client.h"
#include <boost/thread/thread.hpp>
#include "client/game.h"

#define LOGIN_SV_HOST "localhost"
#define LOGIN_SV_PORT 7777

boost::asio::io_context ioc;
instance *current_instance;
// game *g;
std::mutex init_l;
std::string username = "joe";

int main(int argc, char **argv)
{
  // prevent other processes from activating during login
  init_l.lock();
  // artificially create a new game
//  g = new game();
  log_init("client");
  // connect to login server
  current_instance = instance_builder(LOGIN_SV_HOST, LOGIN_SV_PORT);
  // authenticate to the login server
  current_instance -> authenticate(username, "p455w0rd");
  // wait for user card to be transferred
  while(!ucl.contains(username))
  {
    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }
  // finished initialisation
  init_l.unlock();
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
