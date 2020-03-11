#include "lib/log.h"
#include "chat_server/server.h"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include "chat_server/ioc.h"
#include <boost/thread/barrier.hpp>

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
  BOOST_LOG_TRIVIAL(trace) << "loading keys";
  init_crypto(pri);
  BOOST_LOG_TRIVIAL(trace) << "creating server";
  server s(port);
  // block current thread
  BOOST_LOG_TRIVIAL(trace) << "blocking current thread";
  boost::barrier b(2);
  b.wait();
  free(args);
  return 0;
}
