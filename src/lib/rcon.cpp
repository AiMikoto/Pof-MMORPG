#include "lib/rcon.h"
#include "lib/log.h"

boost::asio::io_context rcon_ioc;

rcon::rcon(std::string hostname, int port, std::string tok, rsa_crypto *rsa, aes_crypto *aes)
{
  boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(rcon_ioc);
  BOOST_LOG_TRIVIAL(info) << "attempting rcon to " << hostname << ":" << port;
  boost::asio::ip::tcp::resolver resolver(rcon_ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), hostname, std::to_string(port));
  boost::asio::connect(*sock, endpoint);
  proto = new protocol(sock, rsa);
  call init;
  init.tree().put(OPCODE, OP_CMD);
  init.tree().put("authority.token", tok);
  init.tree().put("command", "rcon");
  init.tree().put("aes.key", aes -> key);
  init.tree().put("aes.iv", aes -> iv);
  proto -> safe_write(init); // uses RSA
  proto -> replace_crypto(aes); // chance crypto to aes
  proto -> start();
}

rcon::~rcon()
{
  delete proto;
}
