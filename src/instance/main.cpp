#include "lib/log.h"
#include "instance/server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include "instance/ioc.h"
#include "instance/crypto.h"

boost::asio::io_context ioc;
database *db;

int main(int argc, char **argv)
{
  std::string pub = "keys/public_key.pem";
  std::string pri = "keys/private_key.pem";
  int port = 7000;
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
  log_init("instance");
  BOOST_LOG_TRIVIAL(trace) << "initialising database";
  db = db_init();
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub, pri);
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server s(port);
  // block current thread
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  std::mutex m;
  m.lock();
  m.lock();
  return 0;
}
