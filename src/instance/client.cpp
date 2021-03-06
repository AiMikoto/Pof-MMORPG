#include "instance/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>
#include "instance/game.h"
#include "instance/misc.h"
#include "lib/chat.h"
#include "instance/crypto.h"
#include "instance/ioc.h"
#include "instance/chat_client.h"
#include "instance/shutdown.h"
#include "instance/token.h"
#include "include/maps.h"

#include "components/phys_collider.h"
#include "components/solid_object.h"
#include "components/meshFilter.h"
#include "components/meshRenderer.h"

client *master = NULL;

boost::asio::io_context irc_context;

client::client(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa, 10)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_SHUTDOWN, boost::bind(&client::handle_shutdown, this, _1));
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
  if(auth_tok != my_token)
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
        ucl.add(uc); // at this point client will start receiving slices
        // TODO: subscribe user to irc
        answer.tree().put("status", true);
        ept.remove(OP_AUTH_TOKEN);
        ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&client::handle_map_change_request, this, _1));
        ept.add(OP_IRC, boost::bind(&client::handle_irc_request, this, _1));
        call scene_transfer;
        scene_transfer.tree().put(OPCODE, OP_SCENE);
        scene_transfer.tree().put_child("data", current -> serialize());
        safe_write(scene_transfer);
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
  bool pub = c.tree().get<bool>("target.public");
  if(pub)
  {
    c.tree().get<int>("target.region");
    c.tree().get<std::string>("target.map");
  }
  else
  {
    c.tree().get<int>("target.instance_id");
  }
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
    // TODO: unsubscribe client from irc
    that -> username = "";
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
  if(command == "irc")
  {
    std::string hostname = c.tree().get<std::string>("target.host");
    int port = c.tree().get<int>("target.port");
    std::string token = c.tree().get<std::string>("target.token");
    boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(irc_context);
    boost::asio::ip::tcp::resolver resolver(irc_context);
    boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), hostname, std::to_string(port));
    boost::asio::connect(*sock, endpoint);
    chat = new chatter(sock);
    call init;
    init.tree().put(OPCODE, OP_CMD);
    init.tree().put("authority.token", token);
    init.tree().put("command", "hello");
    init.tree().put("aes.key", g_aes -> key);
    init.tree().put("aes.iv", g_aes -> iv);
    chat -> safe_write(init); // uses RSA
    chat -> replace_crypto(g_aes); // chance crypto to aes
    call sub_world;
    sub_world.tree().put(OPCODE, OP_IRC_S);
    sub_world.tree().put("meta.target", "w_world");
    chat -> safe_write(sub_world);
    chat -> start();
    return;
  }
  if(command == "load")
  {
    if(is_loaded())
    {
      unload();
    }
    load(c.tree().get<map_t>("map"));
    return;
  }
  if(command == "rcon")
  { // uses RSA
    BOOST_LOG_TRIVIAL(warning) << "received rcon authentication";
    std::string key = c.tree().get<std::string>("aes.key");
    std::string iv = c.tree().get<std::string>("aes.iv");
    aes = new aes_crypto(key, iv);
    replace_crypto(aes);
    return;
  }
  if(command == "edit")
  {
    BOOST_LOG_TRIVIAL(warning) << "enabling editor";
    ept.add(OP_EDIT_SLICER_STATUS, boost::bind(&client::set_slicer, this, _1));
    ept.add(OP_EDIT_SPS, boost::bind(&client::set_sps, this, _1));
    ept.add(OP_EDIT_MOVE_OBJECT, boost::bind(&client::obj_move, this, _1));
    ept.add(OP_EDIT_SCALE_OBJECT, boost::bind(&client::obj_scale, this, _1));
    ept.add(OP_EDIT_ROTATE_OBJECT, boost::bind(&client::obj_rotate, this, _1));
    ept.add(OP_EDIT_SAVE, boost::bind(&client::map_save, this, _1));
    ept.add(OP_EDIT_ADD_OBJ, boost::bind(&client::add_obj, this, _1));
    ept.add(OP_EDIT_ADD_COMP, boost::bind(&client::add_comp, this, _1));
    ept.add(OP_EDIT_DELETE_OBJ, boost::bind(&client::remove_obj, this, _1));
    return;
  }
  BOOST_LOG_TRIVIAL(warning) << "unknown command - " << command;
}

void client::handle_irc_request(call c)
{
  chat_target target = static_cast<chat_target>(c.tree().get<int>("payload.target"));
  bool super = false;
  if(target == world)
  {
    super = true;
    c.tree().put("meta.target", "w_world");
  }
  if(target == party)
  {
    super = true;
    c.tree().put("meta.target", "replace me");
  }
  if(target == guild)
  {
    super = true;
    c.tree().put("meta.target", "replace me");
  }
  if(super)
  {
    chat -> safe_write(c);
  }
  ucl.apply(boost::bind(send_message, _1, c));
}

void client::handle_shutdown(call c)
{
  validate_authority(c.tree().get<std::string>("authority.token"));
  BOOST_LOG_TRIVIAL(trace) << "remote shutdown initiated";
  shutdown();
}

// editor functions

void client::set_slicer(call c)
{
  bool status = c.tree().get<bool>("status");
  slicer_acquire();
  slicer_set_status(status);
  slicer_release();
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}

void client::set_sps(call c)
{
  double sps = c.tree().get<double>("sps");
  slicer_acquire();
  slicer_set_sps(sps);
  slicer_release();
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}

void client::obj_move(call c)
{
  unsigned long long id = c.tree().get<unsigned long long>("id");
  glm::dvec3 pos = engine::vecDeserializer<glm::dvec3, double>(c.tree().get_child("pos"));
  slicer_acquire();
  slicer_move(id, pos);
  slicer_release();
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}

void client::obj_scale(call c)
{
  unsigned long long id = c.tree().get<unsigned long long>("id");
  glm::dvec3 scale = engine::vecDeserializer<glm::dvec3, double>(c.tree().get_child("scale"));
  slicer_acquire();
  slicer_scale(id, scale);
  slicer_release();
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}

void client::obj_rotate(call c)
{
  unsigned long long id = c.tree().get<unsigned long long>("id");
  glm::dvec3 rotation = engine::vecDeserializer<glm::dvec3, double>(c.tree().get_child("rotation"));
  slicer_acquire();
  slicer_rotate(id, rotation);
  slicer_release();
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}

void client::map_save(call c)
{
  try
  {
    map_t map = c.tree().get<map_t>("name");
    save(map);
  }
  catch(std::exception &e)
  {
    save();
  }
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}

void client::add_obj(call c)
{
  c.tree().put(OPCODE, OP_EDIT_CB);
  c.tree().put("id", game_inject_object());
  safe_write(c);
}

void client::add_comp(call c)
{
  engine::Component *comp = NULL;
  unsigned long long id = c.tree().get<unsigned long long>("target");
  std::string comp_type = c.tree().get<std::string>("recipe.type");
  if(comp_type == "solid_object")
  {
    comp = new solid_object();
  }
  if(comp_type == "physical_collider")
  {
    collider_t c_type;
    std::string shape = c.tree().get<std::string>("recipe.shape");
    if(shape == "sphere")
    {
      c_type = sphere;
    }
    if(shape == "box")
    {
      c_type = box;
    }
    if(shape == "capsule")
    {
      c_type = caps;
    }
    comp = new physical_collider({2, 2, 2}, c_type);
  }
  if(comp_type == "mesh_filter")
  {
    std::string path = c.tree().get<std::string>("recipe.path");
    comp = new engine::MeshFilter(path);
  }
  if(comp_type == "mesh_renderer")
  {
    comp = new engine::MeshRenderer();
  }
  slicer_acquire();
  slicer_inject_component(id, comp);
  slicer_release();
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}

void client::remove_obj(call c)
{
  unsigned long long id = c.tree().get<unsigned long long>("id");
  game_delete_object(id);
  c.tree().put(OPCODE, OP_EDIT_CB);
  safe_write(c);
}
