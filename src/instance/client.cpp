#include "instance/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>
#include "instance/game.h"

client *master;

std::string my_tok = "fish"; // TODO: export this

rsa_crypto *g_rsa;

void init_crypto(std::string priv)
{
  BIO *keybio = BIO_new(BIO_s_file());
  BIO_read_filename(keybio, priv.c_str());
  g_rsa = new rsa_crypto(NULL, keybio);
}

client::client(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa, 10)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_AUTH_TOKEN, boost::bind(&client::handle_auth, this, _1));
  ept.add(OP_CMD, boost::bind(&client::handle_cmd, this, _1));
  start();
}

client::~client()
{
}

// throws instead of returning.
void client::validate_authority(std::string auth_tok)
{
  if(auth_tok != my_tok)
  {
    throw std::logic_error("wrong authority token");
  }
}

void client::uc_transfer(call c)
{
  validate_authority(c.tree().get<std::string>("authority.token"));
  user_card uc;
  uc.tree() = c.tree().get_child("data");
  BOOST_LOG_TRIVIAL(trace) << "received user card(pending): " << uc.tree().get<std::string>("user.name");
  uclp.add(uc);
}

void client::handle_auth(call c)
{ // uses RSA
  username = c.tree().get<std::string>("login.username");
  std::string tok = c.tree().get<std::string>("login.token");
  std::string key = c.tree().get<std::string>("aes.key");
  std::string iv = c.tree().get<std::string>("aes.iv");
  aes = new aes_crypto(key, iv);
  replace_crypto(aes);
  BOOST_LOG_TRIVIAL(trace) << "extracted login token - " << tok;
  call answer;
  user_card uc;
  answer.tree().put(OPCODE, OP_AUTH_TOKEN);
  answer.tree().put("status", false);
  for(int trials = 1; trials <= 3; trials++)
  {
    if(uclp.contains(username))
    {
      uc = uclp.get(username);
      if(uc.tree().get<std::string>("user.token") == tok)
      {
        BOOST_LOG_TRIVIAL(trace) << "user login successful - " << username;
        uclp.remove(username);
        uc.aux = (void *) this;
        ucl.add(uc);
        answer.tree().put("status", true);
        ept.remove(OP_AUTH_TOKEN);
        ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&client::handle_map_change_request, this, _1));
        // TODO: populate calls
        break;
      }
      else
      {
        break;
      }
    }
    else
    {
      boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
  }
  safe_write(answer);
  if(!answer.tree().get<bool>("status"))
  { // terminate upon failed authentication
    this -> close();
  }
}

void client::handle_map_change_request(call c)
{
  BOOST_LOG_TRIVIAL(trace) << "client requested map change";
  c.tree().put_child("card", ucl.get(username).tree());
  master -> safe_write(c);
}

void client::handle_map_change_request_cb(call c)
{
  std::string uname = c.tree().get<std::string>("card.user.name");
  user_card uc = ucl.get(uname);
  client *that = (client *)(uc.aux);
  call answer;
  answer.tree().put(OPCODE, OP_REQUEST_CHANGE_MAP);
  answer.tree().put("status", c.tree().get<bool>("status"));
  that -> safe_write(answer);
  if(c.tree().get<bool>("status"))
  {
    BOOST_LOG_TRIVIAL(trace) << "server approved map change";
    // if authorised, card will be transferred to new instance
    ucl.remove(uname);
    call move;
    move.tree().put(OPCODE, OP_MOVE);
    move.tree().put("target.host", c.tree().get<std::string>("target.host"));
    move.tree().put("target.port", c.tree().get<int>("target.port"));
    that -> safe_write(move);
    that -> close();
  }
}

void client::handle_cmd(call c)
{
  validate_authority(c.tree().get<std::string>("authority.token"));
  std::string command = c.tree().get<std::string>("command");
  if(command == "init")
  { // uses RSA
    BOOST_LOG_TRIVIAL(info) << "updated master to " << socket -> remote_endpoint().address().to_string();
    std::string key = c.tree().get<std::string>("aes.key");
    std::string iv = c.tree().get<std::string>("aes.iv");
    aes = new aes_crypto(key, iv);
    replace_crypto(aes);
    master = this;
    ept.add(OP_REQUEST_CHANGE_MAP_CB, boost::bind(&client::handle_map_change_request_cb, this, _1));
    ept.add(OP_UC_TRANS_ALL, boost::bind(&client::uc_transfer, this, _1));
    return;
  }
  BOOST_LOG_TRIVIAL(warning) << "unknown command - " << command;
}
