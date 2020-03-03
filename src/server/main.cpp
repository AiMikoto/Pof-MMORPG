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
  std::string pub = "setup/public_key.pem";
  std::string pri = "setup/private_key.pem";
  int port = 7777;
  // parsing arguments;
  std::string args[argc];
  for(int i = 0; i < argc; i++)
  {
    args[i] = std::string(argv[i]);
  }
  for(int i = 1; i < argc; i++)
  {
    if(args[i] == "-pub")
    {
      pub = args[++i];
      continue;
    }
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
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub, pri);
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
