#include "lib/log.h"
#include "chat_server/server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include "chat_server/ioc.h"
#include "chat_server/shutdown.h"
#include "chat_server/rooms.h"
#include "chat_server/crypto.h"

#ifdef __linux__
#include <csignal>
#endif

boost::asio::io_context ioc;

int main(int argc, char **argv)
{
  std::string pri = "keys/private_key.pem";
  int port = 1231;
  // parsing arguments;
  std::string *args = new std::string[argc];
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
  log_init("chat_server");
#ifdef __linux__
  BOOST_LOG_TRIVIAL(trace) << "loading handler for SIGINT";
  std::signal(SIGINT, shutdown);
#endif
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pri);
  BOOST_LOG_TRIVIAL(trace) << "initialising chat rooms";
  chat_init(); 
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server *s = new server(port);
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  main_barrier.wait();
  BOOST_LOG_TRIVIAL(trace) << "cleaning up server";
  delete s;
  BOOST_LOG_TRIVIAL(trace) << "destroying chat rooms";
  chat_destroy();
  BOOST_LOG_TRIVIAL(trace) << "destroying keys";
  destroy_crypto();
  BOOST_LOG_TRIVIAL(trace) << "deleting argument array";
  delete[] args;
  return 0;
}
