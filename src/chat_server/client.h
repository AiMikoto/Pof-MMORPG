#ifndef CHAT_SERVER_CLIENT_H
#define CHAT_SERVER_CLIENT_H

#include "lib/protocol.h"

void init_crypto(std::string priv);

class client:public protocol
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
private:
  void validate_authority(std::string token);
  void handle_cmd(call c);
  void handle_shutdown(call c);
  void handle_irc_request(call c);
  void handle_subscribe(call c);
  void handle_unsubscribe(call c);
};

#endif // CHAT_SERVER_CLIENT_H
