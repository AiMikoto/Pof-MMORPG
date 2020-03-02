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
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto("setup/public_key.pem", "setup/private_key.pem");
  int port = std::atoi(argv[1]);
  BOOST_LOG_TRIVIAL(trace) << "creating instances";
  populate_pins();
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server s(port);
  BOOST_LOG_TRIVIAL(trace) << "blocking thread";
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
