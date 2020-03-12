#include "server/hosts.h"
#include "include/regions.h"

boost::property_tree::ptree hosts;

void add(std::string type, region_t reg, std::string host, int port)
{
  boost::property_tree::ptree entry;
  entry.put("host", host);
  entry.put("port", port);
  entry.put("region", reg);
  int count = hosts.get<int>("instance.count");
  hosts.put("instance.count", count + 1);
  hosts.put_child("instance." + std::to_string(count), entry);
}

void init_hosts()
{
  hosts.put("instance.count", 0);
  add("instance", REG_EU, "localhost", 7000);
  hosts.put("chat.count", 0);
  add("chat", REG_EU, "localhost", 1231);
}

void clear_hosts()
{
  hosts.clear();
}
