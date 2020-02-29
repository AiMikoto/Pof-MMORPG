#include "lib/call.h"
#include "lib/log.h"
#include "client/ioc.h"
#include "client/client.h"
#include <boost/thread/thread.hpp>

boost::asio::io_context ioc;

int main(int argc, char **argv)
{
  log_init("client");
  instance *ins = instance_builder("localhost", 7777);
  
  ins -> authenticate("joe", "p455w0rd");
  boost::this_thread::sleep( boost::posix_time::seconds(10) );
  ins -> close();
  delete ins;
  return 0;
}
