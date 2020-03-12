#include "server/hosts.h"
#include "lib/protocol.h"
#include "server/crypto.h"
#include "server/ioc.h"

boost::property_tree::ptree hosts;

void add(std::string type, region_t reg, std::string host, int port, std::string token)
{
  boost::property_tree::ptree entry;
  entry.put("host", host);
  entry.put("port", port);
  entry.put("region", reg);
  entry.put("token", token);
  int count = hosts.get<int>("instance.count");
  hosts.put(type + ".count", count + 1);
  hosts.put_child(type+ "." + std::to_string(count), entry);
}

void init_hosts()
{
  hosts.put("instance.count", 0);
  add("instance", REG_EU, "localhost", 7000, "fish");
  hosts.put("chat.count", 0);
  add("chat", REG_EU, "localhost", 1231, "lion");
}

void take_down(std::string target)
{
  boost::property_tree::ptree instance = hosts.get_child(target);
  std::string tok = instance.get<std::string>("token");
  std::string hostname = hostname = instance.get<std::string>("host");
  int port = instance.get<int>("port");
  boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(ioc);
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), hostname, std::to_string(port));
  boost::asio::connect(*sock, endpoint);
  protocol *p = new protocol(sock, g_rsa, -1);
  call init;
  init.tree().put(OPCODE, OP_CMD);
  init.tree().put("authority.token", tok);
  init.tree().put("command", "rcon");
  init.tree().put("aes.key", g_aes -> key);
  init.tree().put("aes.iv", g_aes -> iv);
  p -> safe_write(init); // uses RSA
  p -> replace_crypto(g_aes); // chance crypto to aes
  call destroy;
  destroy.tree().put(OPCODE, OP_SHUTDOWN);
  destroy.tree().put("authority.token", tok);
  p -> safe_write(destroy);
  init.tree().put(OPCODE, OP_CMD);
}

void clear_hosts()
{
  hosts.clear();
}
