#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include "lib/protocol.h"

void init_crypto_client(std::string priv);

class client:public protocol
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
private:
  void validate_authority(std::string token);
  void handle_auth(call c);
  void handle_cmd(call c);
  void handle_shutdown(call c);
};

#endif // SERVER_CLIENT_H
