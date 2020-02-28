#include "lib/call.h"
#include "client/ioc.h"
#include "client/client.h"

boost::asio::io_context ioc;

int main(int argc, char **argv)
{
  instance *ins = new instance("localhost", 7777);
  
  ins -> periodic();
  ins -> close();
  return 0;
}
