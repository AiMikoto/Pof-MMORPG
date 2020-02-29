#ifndef SERVER_INSTANCES_H
#define SERVER_INSTANCES_H

#include <map>
#include <string>
#include "lib/protocol.h"

// all instances have a static hostname and a port assigned to then
// a single hostname can be shared by multiple instances as long as
// the combination of hostname and port is unique
// all instances have a unique id and an authority token


class instance:public protocol
{
public:
  instance(boost::asio::ip::tcp::socket *sock);
};

class instance_info
{
public:
  instance_info(std::string auth_tok, std::string hostname, int port);
  std::string uuid;
  std::string auth_tok;
  std::string hostname;
  int port;
  instance *in;
};

// public instances have a region-specific channel and a map
// public instances are STATIC - they never change region or map

class public_instance:public instance_info
{
public:
  public_instance(std::string auth_tok, std::string hostname, int port, int channel, std::string map);
  int channel;
  std::string map;
};

extern std::map<int, std::map<std::string, public_instance*>> pins;

// dynamic instances are created/destroyed on demand, their map is game-defined
// TODO: dynamic instances

instance *instance_builder(instance_info ini);

void populate_pins();

#endif
