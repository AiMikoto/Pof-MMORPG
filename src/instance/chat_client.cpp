#include "instance/chat_client.h"
#include "instance/crypto.h"
#include "instance/misc.h"
#include "instance/game.h"
#include <boost/bind.hpp>

chatter *chat;

chatter::chatter(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa, -1)
{
  ept.add(OP_IRC, boost::bind(&chatter::handle_irc_request, this, _1));
}

void chatter::handle_irc_request(call c)
{
  ucl.apply(boost::bind(send_message, _1, c));
}
