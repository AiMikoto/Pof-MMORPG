#include "server/server.h"
#include "lib/log.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include "server/ioc.h"
#include "server/instances.h"
#include "server/crypto.h"
#include <boost/thread/barrier.hpp>

database *db;
boost::asio::io_context ioc;

int main(int argc, char **argv)
{
  log_init("server");
  std::string pub = "keys/public_key.pem";
  std::string pri = "keys/private_key.pem";
  int port = 7777;
  // parsing arguments;
  std::string *args = new std::string[argc];
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
  BOOST_LOG_TRIVIAL(trace) << "initialising database";
  db = db_init();
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub, pri);
  BOOST_LOG_TRIVIAL(trace) << "creating instances";
  populate_dins();
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server s(port);
  BOOST_LOG_TRIVIAL(trace) << "blocking thread";
  boost::barrier b(2);
  b.wait();
  return 0;
}
