#ifndef SERVER_INSTANCES_H
#define SERVER_INSTANCES_H

#include <map>
#include <string>
#include "lib/protocol.h"
#include "include/maps.h"
#include "include/regions.h"
#include "common/user_card.h"
#include "lib/database.h"

#define instance_id_t int

class instance:public protocol
{
public:
  instance(boost::asio::ip::tcp::socket *sock, instance_id_t id);
private:
  void handle_map_change_request(call c);
  void handle_deactivate(call c);
  void handle_reactivate(call c);
  instance_id_t id;
};

class instance_info
{
public:
  instance_info(region_t reg, std::string auth_tok, std::string hostname, int port, instance_id_t id);
  ~instance_info();
  void transfer_user_card(user_card uc);
  region_t reg;
  std::string auth_tok;
  std::string hostname;
  int port;
  instance *in;
};

extern std::map<instance_id_t, instance_info *> fins; // free instances
extern std::map<instance_id_t, instance_info *> pins; // pending instances
extern std::map<instance_id_t, instance_info *> ains; // active instances

instance_info *get_pub_in(region_t reg, map_t map);

extern instance_id_t instance_counter;

void populate_dins();

void disable_dins();

void destroy_dins();

#endif // SERVER_INSTANCES_H
