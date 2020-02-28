#include "instance/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>


client::client(boost::asio::ip::tcp::socket *sock):protocol(sock)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
}

client::~client()
{
}

void client::periodic()
{
  BOOST_LOG_TRIVIAL(info) << "sending ping";
  call ping;
  ping.tree().add(OPCODE, OP_PING);
  write_call(socket, ping);
}
