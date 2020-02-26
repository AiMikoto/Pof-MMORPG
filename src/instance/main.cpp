#include "lib/log.h"
#include "instance/server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <mutex>

int main(int argc, char **argv)
{
  boost::asio::io_context ioc;
  if(argc < 2)
  {
    std::cerr << "improper call of instance creation" << std::endl;
    return 1;
  }
  log_init("instance");
  // extract arguments
  int port = std::atoi(argv[1]);
  // create a server 
  server s(ioc, port);
  // block current thread
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
