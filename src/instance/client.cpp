#include "instance/client.h"

client::client(boost::asio::ip::tcp::socket *sock)
{
  socket = sock;
}

client::~client()
{
  socket -> close();
  delete socket;
}
