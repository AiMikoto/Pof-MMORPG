#include "manipulator/crypto.h"
#include "manipulator/manipulator.h"
#include <boost/thread/barrier.hpp>
#include "core/utils.h"

manipulator::manipulator(std::string hostname, int port, std::string tok):rcon(hostname, port, tok, g_rsa, g_aes)
{
  call init;
  init.tree().put(OPCODE, OP_CMD);
  init.tree().put("authority.token", tok);
  init.tree().put("command", "edit");
  // get editor privileges
  proto -> safe_write(init);
}

unsigned long long manipulator::spawn()
{
}

void set_slicer_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::set_slicer(bool value)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(set_slicer_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_SLICER_STATUS);
  c.tree().put("status", value);
  proto -> safe_write(c);
  bar.wait();
}

void set_sps_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::set_sps(double value)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(set_sps_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_SPS);
  c.tree().put("sps", value);
  proto -> safe_write(c);
  bar.wait();
}

void obj_move_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::obj_move(unsigned long long id, glm::dvec3 pos)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_move_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_MOVE_OBJECT);
  c.tree().put("id", id);
  c.tree().put_child("pos", engine::vecSerializer(pos));
  proto -> safe_write(c);
  bar.wait();
}

void save_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::save()
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(save_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_SAVE);
  proto -> safe_write(c);
  bar.wait();
}

void manipulator::save(map_t map)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(save_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_SAVE);
  c.tree().put("map", map);
  proto -> safe_write(c);
  bar.wait();
}
