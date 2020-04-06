#include "manipulator/crypto.h"
#include "manipulator/manipulator.h"
#include <boost/thread/barrier.hpp>

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
  bool status;
  call c;
  proto -> ept.add(OP_EDIT_CB, boost::bind(set_slicer_cb, &bar, _1));
  c.tree().put(OPCODE, OP_EDIT_SLICER_STATUS);
  c.tree().put("status", value);
  proto -> safe_write(c);
  bar.wait();
}
