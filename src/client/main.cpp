#include <boost/asio.hpp>
#include "lib/call.h"

int main(int argc, char **argv)
{
  boost::asio::io_context io_context;

  boost::asio::ip::tcp::resolver resolver(io_context);
  boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(boost::asio::ip::tcp::v4(), argv[1], argv[2]);
  boost::asio::ip::tcp::socket s(io_context);
  boost::asio::connect(s, endpoints);
  call c;
  write_call(&s, c);
  while(1);
  return 0;
}
