#ifndef LIB_RCON_H
#define LIB_RCON_H

#include "lib/protocol.h"

class rcon
{
public:
  rcon(std::string hostname, int port, std::string tok, rsa_crypto *rsa, aes_crypto *aes);
  ~rcon();
  protocol *proto;
};

#endif // LIB_RCON_H
