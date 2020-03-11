#ifndef INSTANCE_CHAT_CLIENT_H
#define INSTANCE_CHAT_CLIENT_H

#include "lib/protocol.h"

class chatter:public protocol
{
public:
  chatter(boost::asio::ip::tcp::socket *sock);
private:
  void handle_irc_request(call c);
};

extern chatter *chat;

#endif
