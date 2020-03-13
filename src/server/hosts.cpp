#include "server/hosts.h"
#include "lib/protocol.h"
#include "server/crypto.h"
#include "server/ioc.h"
#include "lib/log.h"
#include "lib/rcon.h"

boost::property_tree::ptree hosts;

boost::asio::io_context hosts_ioc;

void add(std::string type, region_t reg, std::string host, int port, std::string token)
{
  boost::property_tree::ptree entry;
  entry.put("host", host);
  entry.put("port", port);
  entry.put("region", reg);
  entry.put("token", token);
  int count = hosts.get<int>(type + ".count");
  hosts.put(type + ".count", count + 1);
  hosts.put_child(type+ "." + std::to_string(count), entry);
}

void init_hosts()
{
  hosts.put("instance.count", 0);
  add("instance", REG_EU, "127.0.0.1", 7000, "fish");
  hosts.put("chat.count", 0);
  add("chat", REG_EU, "127.0.0.1", 1231, "lion");
}

void take_down(std::string target)
{
  boost::property_tree::ptree instance = hosts.get_child(target);
  std::string tok = instance.get<std::string>("token");
  std::string hostname = instance.get<std::string>("host");
  int port = instance.get<int>("port");
  rcon r(hostname, port, tok, g_rsa, g_aes);
  call destroy;
  destroy.tree().put(OPCODE, OP_SHUTDOWN);
  destroy.tree().put("authority.token", tok);
  r.proto -> safe_write(destroy);
}

void clear_hosts()
{
  hosts.clear();
}
