#include "server/server.h"
#include "lib/log.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include "server/ioc.h"
#include "server/instances.h"

boost::asio::io_context ioc;

int main(int argc, char **argv)
{
  log_init("server");
  int port = std::atoi(argv[1]);
  // create instances
  BOOST_LOG_TRIVIAL(debug) << "creating instances";
  populate_pins();
  // create a server 
  BOOST_LOG_TRIVIAL(debug) << "creating server";
  server s(port);
  // block current thread
  BOOST_LOG_TRIVIAL(debug) << "blocking thread";
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
