#include "server/server.h"
#include "lib/log.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <mutex>

int main(int argc, char **argv)
{
  boost::asio::io_context ioc;
  log_init("server");
  int port = std::atoi(argv[1]);
  // create a server 
  server s(ioc, port);
  // block current thread
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
