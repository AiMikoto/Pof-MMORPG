#include "lib/log.h"
#include "instance/server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include "instance/ioc.h"
#include "instance/crypto.h"
#include <boost/thread/barrier.hpp>
#include "instance/shutdown.h"

#ifdef __linux__
#include <csignal>
#endif

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
#ifdef __linux__
  BOOST_LOG_TRIVIAL(trace) << "loading handler for SIGINT";
  std::signal(SIGINT, shutdown);
#endif
  BOOST_LOG_TRIVIAL(trace) << "initialising database";
  db = db_init();
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub, pri);
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server *s = new server(port);
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  main_barrier.wait();
  BOOST_LOG_TRIVIAL(trace) << "stopping io context";
  ioc.stop();
  BOOST_LOG_TRIVIAL(trace) << "cleaning up server";
  delete s;
  BOOST_LOG_TRIVIAL(trace) << "destroying keys";
  destroy_crypto();
  BOOST_LOG_TRIVIAL(trace) << "closing database";
  delete db;
  BOOST_LOG_TRIVIAL(trace) << "deleting argument array";
  delete[] args;
  return 0;
}
