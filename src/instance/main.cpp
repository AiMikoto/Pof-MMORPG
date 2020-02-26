#include "instance/server.h"
#include <boost/asio.hpp>

int main()
{
  boost::asio::io_context ioc;
  server s(ioc, 7777);
  while(1){};
  return 0;
}
