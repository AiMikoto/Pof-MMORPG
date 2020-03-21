#include "server/server.h"
#include "lib/log.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include "server/ioc.h"
#include "server/instances.h"
#include "server/crypto.h"
#include <boost/thread/barrier.hpp>
#include "server/shutdown.h"
#include "server/hosts.h"
#include "server/token.h"

#ifdef __linux__
#include <csignal>
#endif

std::string my_token = "rubber";

boost::asio::io_context resolver_context;
boost::asio::ip::tcp::resolver resolver(resolver_context);

database *db;

int main(int argc, char **argv)
{
  log_init("server");
  std::string pub = "keys/public_key.pem";
  std::string pri = "keys/private_key.pem";
  int port = 7777;
  bool sins = true;
  // parsing arguments;
  std::string *args = new std::string[argc];
  for(int i = 0; i < argc; i++)
  {
    args[i] = std::string(argv[i]);
  }
  for(int i = 1; i < argc; i++)
  {
    if(args[i] == "-no_static_ins")
    {
      sins = false;
      continue;
    }
    if(args[i] == "-tok")
    {
      my_token = args[++i];
      continue;
    }
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
#ifdef __linux__
  BOOST_LOG_TRIVIAL(trace) << "loading handler for SIGINT";
  std::signal(SIGINT, shutdown);
#endif
  if(sins)
  {
    BOOST_LOG_TRIVIAL(trace) << "initialising host database";
    init_hosts();
  }
  BOOST_LOG_TRIVIAL(trace) << "initialising database";
  db = db_init();
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pub, pri);
  BOOST_LOG_TRIVIAL(trace) << "creating instances";
  populate_dins();
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server *s = new server(port);
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  main_barrier.wait();
  BOOST_LOG_TRIVIAL(trace) << "cleaning up server";
  delete s;
  // Authentication is disabled at this point
  BOOST_LOG_TRIVIAL(trace) << "disabling dynamic instance interaction";
  disable_dins();
  // At this point, instances can safely be terminated
  BOOST_LOG_TRIVIAL(trace) << "destroying dynamic instances";
  destroy_dins();
  // At this point, there are no more users in the system
  take_down("chat.0");
  BOOST_LOG_TRIVIAL(trace) << "destroying keys";
  destroy_crypto();
  BOOST_LOG_TRIVIAL(trace) << "closing database";
  delete db;
  BOOST_LOG_TRIVIAL(trace) << "destroying host database";
  clear_hosts();
  BOOST_LOG_TRIVIAL(trace) << "deleting argument array";
  delete[] args;
  return 0;
}
