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
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto("setup/private_key.pem");
  BOOST_LOG_TRIVIAL(trace) << "extracting arguments";
  int port = std::atoi(argv[1]);
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server s(ioc, port);
  // block current thread
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
