#include "server/instances.h"
#include "server/ioc.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "server/crypto.h"
#include "lib/log.h"
#include "server/hosts.h"

boost::asio::io_context instances_ioc;

bool shuttingdown = false;

std::map<instance_id_t, instance_info *> fins;
std::map<instance_id_t, instance_info *> pins;
std::map<instance_id_t, instance_info *> ains;

instance_id_t instance_counter = 0;

instance::instance(boost::asio::ip::tcp::socket *sock, instance_id_t id):protocol(sock, g_rsa, -1)
{
  this -> id = id;
  ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&instance::handle_map_change_request, this, _1));
  ept.add(OP_INSTANCE_MANAGEMENT_DEACTIVATE, boost::bind(&instance::handle_deactivate, this, _1));
  ept.add(OP_INSTANCE_MANAGEMENT_REACTIVATE, boost::bind(&instance::handle_reactivate, this, _1));
}

instance_info *get_active_instance(instance_id_t id)
{
  if(pins.find(id) != pins.end())
  {
    BOOST_LOG_TRIVIAL(trace) << "reactivating instance " << id;
    ains[id] = pins[id];
    pins.erase(id);
  }
  if(ains.find(id) != ains.end())
  {
    return ains[id];
  }
  return NULL;
}

void instance::handle_map_change_request(call c)
{
  c.tree().put(OPCODE, OP_REQUEST_CHANGE_MAP_CB);
  c.tree().put("status", false);
  if(shuttingdown)
  {
    this -> safe_write(c);
  }
  bool pub = c.tree().get<bool>("target.public");
  instance_info *insi;
  if(pub)
  {
    region_t reg = c.tree().get<region_t>("target.region");
    map_t map = c.tree().get<map_t>("target.map");
    insi = get_pub_in(reg, map);
  }
  else
  {
    instance_id_t id = c.tree().get<instance_id_t>("target.instance_id");
    insi = get_active_instance(id);
  }
  user_card uc;
  uc.tree() = c.tree().get_child("card");
  if(insi)
  {
    c.tree().put("status", true);
    c.tree().put("target.host", insi -> hostname);
    c.tree().put("target.port", insi -> port);
    insi -> transfer_user_card(uc);
  }
  else
  {
    c.tree().put("status", false);
  }
  this -> safe_write(c);
}

void handle_free(instance_id_t id)
{
  boost::this_thread::sleep(boost::posix_time::seconds(30));
  if(pins.find(id) != pins.end())
  {
  BOOST_LOG_TRIVIAL(trace) << "freeing instance " << id;
    fins[id] = pins[id];
    pins.erase(id);
  }
}

void instance::handle_deactivate(call c)
{
  if(shuttingdown)
  {
    return;
  }
  BOOST_LOG_TRIVIAL(trace) << "deactivating instance " << id;
  pins[id] = ains[id];
  ains.erase(id);
  boost::thread t_cleanup(boost::bind(handle_free, id));
}

void instance::handle_reactivate(call c)
{
  if(shuttingdown)
  {
    return;
  }
  if(pins.find(id) != pins.end())
  {
    BOOST_LOG_TRIVIAL(trace) << "reactivating instance " << id;
    ains[id] = pins[id];
    pins.erase(id);
  }
}

instance_info::instance_info(region_t reg, std::string auth_tok, std::string hostname, int port, instance_id_t id)
{
  this -> reg = reg;
  this -> auth_tok = auth_tok;
  this -> hostname = hostname;
  this -> port = port;
  boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(instances_ioc);
  boost::asio::ip::tcp::resolver resolver(instances_ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), this -> hostname, std::to_string(this -> port));
  boost::asio::connect(*sock, endpoint);
  this -> in = new instance(sock, id);
  call init;
  init.tree().put(OPCODE, OP_CMD);
  init.tree().put("authority.token", this -> auth_tok);
  init.tree().put("command", "init");
  init.tree().put("aes.key", g_aes -> key);
  init.tree().put("aes.iv", g_aes -> iv);
  this -> in -> safe_write(init); // uses RSA
  this -> in -> replace_crypto(g_aes); // chance crypto to aes
  call chat_init;
  chat_init.tree().put(OPCODE, OP_CMD);
  chat_init.tree().put("authority.token", this -> auth_tok);
  chat_init.tree().put("command", "irc");
  chat_init.tree().put("target.host", "localhost");
  chat_init.tree().put("target.port", 1231);
  chat_init.tree().put("target.token", "lion");
  this -> in -> safe_write(chat_init);
  this -> in -> start();
}

instance_info::~instance_info()
{
  call destroy;
  destroy.tree().put(OPCODE, OP_SHUTDOWN);
  destroy.tree().put("authority.token", this -> auth_tok);
  this -> in -> safe_write(destroy);
  delete this -> in;
}

void instance_info::transfer_user_card(user_card uc)
{
  call uc_transfer;
  uc_transfer.tree().put(OPCODE, OP_UC_TRANS_ALL);
  uc_transfer.tree().put_child("data", uc.tree());
  uc_transfer.tree().put("authority.token", this -> auth_tok);
  this -> in -> safe_write(uc_transfer);
}

std::map<region_t, std::map<map_t, instance_id_t>> pub_ins;

instance_id_t instance_allocate(region_t reg, map_t map)
{
  BOOST_LOG_TRIVIAL(trace) << "creating public instance";
  instance_id_t insid;
  instance_info *insi;
  for(auto it = fins.begin(); it != fins.end(); it++)
  {
    if(it -> second -> reg == reg)
    {
      insid = it -> first;
      insi = it -> second;
      break;
    }
  }
  fins.erase(insid);
  ains[insid] = insi;
  call c;
  c.tree().put(OPCODE, OP_CMD);
  c.tree().put("authority.token", insi-> auth_tok);
  c.tree().put("command", "load");
  c.tree().put("map", map);
  insi -> in -> safe_write(c);
  return insid;
}

instance_info *get_pub_in(region_t reg, map_t map)
{
  if((pub_ins.find(reg) == pub_ins.end()) || (pub_ins[reg].find(map) == pub_ins[reg].end()))
  {
    pub_ins[reg][map] = instance_allocate(reg, map);
  }
  if(fins.find(pub_ins[reg][map]) != fins.end())
  {
    pub_ins[reg][map] = instance_allocate(reg, map);
  }
  return get_active_instance(pub_ins[reg][map]);
}

void populate_dins()
{
  int count = hosts.get<int>("instance.count");
  for(int i = 0; i < count; i++)
  {
    boost::property_tree::ptree instance = hosts.get_child("instance." + std::to_string(i));
    fins[instance_counter++] = new instance_info(instance.get<region_t>("region"), instance.get<std::string>("token"), instance.get<std::string>("host"), instance.get<int>("port"), instance_counter);
  }
}

void ignore(call c)
{
}

void disable_dins()
{
  shuttingdown = true;
}

void destroy_dins()
{
  for(auto ins:ains)
  {
    BOOST_LOG_TRIVIAL(trace) << "destroying active instance";
    delete ins.second;
  }
  for(auto ins:pins)
  {
    BOOST_LOG_TRIVIAL(trace) << "destroying pending instance";
    delete ins.second;
  }
  for(auto ins:fins)
  {
    BOOST_LOG_TRIVIAL(trace) << "destroying free instance";
    delete ins.second;
  }
  BOOST_LOG_TRIVIAL(trace) << "clearing public instance index";
  pub_ins.clear();
}
