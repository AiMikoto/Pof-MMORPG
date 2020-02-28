#include "lib/call.h"
#include "lib/log.h"
#include "client/ioc.h"
#include "client/client.h"

boost::asio::io_context ioc;

int main(int argc, char **argv)
{
  log_init("client");
  instance *ins = instance_builder("localhost", 7777);
  
  ins -> periodic();
  ins -> close();
  return 0;
}
