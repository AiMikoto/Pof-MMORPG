#include "lib/log.h"
#include "instance/server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <mutex>

boost::asio::io_context ioc;

int main(int argc, char **argv)
{
  boost::asio::io_context ioc;
  std::string pri = "setup/private_key.pem";
  int port = 7000;
  // parsing arguments;
  std::string args[argc];
  for(int i = 0; i < argc; i++)
  {
    args[i] = std::string(argv[i]);
  }
  for(int i = 1; i < argc; i++)
  {
    if(args[i] == "-priv")
    {
      pri = args[++i];
      continue;
    }
    if(args[i] == "-port")
    {
      port = std::stoi(args[++i]);
      continue;
    }
    BOOST_LOG_TRIVIAL(warning) << "unknown parameter " << args[i];
  }
  log_init("instance");
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pri);
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server s(port);
  // block current thread
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
