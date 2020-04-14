#include "client/crypto.h"
#include "client/manipulator.h"
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

void spawn_cb(boost::barrier *bar, oid_t *ret, call c)
{
  *ret = oid_t(c.tree().get<std::string>("id"));
  bar -> wait();
}

oid_t manipulator::spawn()
{
  boost::barrier bar(2);
  call c;
  oid_t ret;
  proto -> ept.add(OP_EDIT_CB, boost::bind(spawn_cb, &bar, &ret, _1));
  c.tree().put(OPCODE, OP_EDIT_ADD_OBJ);
  proto -> safe_write(c);
  bar.wait();
  return ret;
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

void manipulator::obj_move(oid_t &id, glm::dvec3 pos)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_move_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_MOVE_OBJECT);
  c.tree().put_child("id", id.encode());
  c.tree().put_child("pos", engine::vecSerializer(pos));
  proto -> safe_write(c);
  bar.wait();
}

void obj_scale_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::obj_scale(oid_t &id, glm::dvec3 scale)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_scale_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_SCALE_OBJECT);
  c.tree().put_child("id", id.encode());
  c.tree().put_child("scale", engine::vecSerializer(scale));
  proto -> safe_write(c);
  bar.wait();
}

void obj_rotate_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::obj_rotate(oid_t &id, glm::dvec3 rotation)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_rotate_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_ROTATE_OBJECT);
  c.tree().put_child("id", id.encode());
  c.tree().put_child("rotation", engine::vecSerializer(rotation));
  proto -> safe_write(c);
  bar.wait();
}

void obj_delete_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::obj_delete(oid_t &id)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_delete_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_DELETE_OBJ);
  c.tree().put_child("id", id.encode());
  proto -> safe_write(c);
  bar.wait();
}

void obj_attach_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::obj_attach(oid_t &from, oid_t &to)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_attach_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_ATTACH_OBJ);
  c.tree().put_child("from", from.encode());
  c.tree().put_child("to", to.encode());
  proto -> safe_write(c);
  bar.wait();
}

void comp_add_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::comp_add(oid_t &target, boost::property_tree::ptree recipe)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(comp_add_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_ADD_COMP);
  c.tree().put_child("target", target.encode());
  c.tree().put_child("recipe", recipe);
  proto -> safe_write(c);
  bar.wait();
}

void obj_meta_cb(boost::barrier *bar, call c)
{
  bar -> wait();
}

void manipulator::obj_rename(oid_t &target, std::string name)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_meta_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_META_OBJ);
  c.tree().put_child("id", target.encode());
  c.tree().put("meta.name", name);
  proto -> safe_write(c);
  bar.wait();
}

void manipulator::obj_tag(oid_t &target, std::string tag)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_meta_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_META_OBJ);
  c.tree().put_child("id", target.encode());
  c.tree().put("meta.tag+", tag);
  proto -> safe_write(c);
  bar.wait();
}

void manipulator::obj_untag(oid_t &target, std::string tag)
{
  boost::barrier bar(2);
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(obj_meta_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_META_OBJ);
  c.tree().put_child("id", target.encode());
  c.tree().put("meta.tag-", tag);
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
  c.tree().put("name", map);
  proto -> safe_write(c);
  bar.wait();
}
