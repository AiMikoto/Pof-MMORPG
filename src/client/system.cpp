#include "client/system.h"
#include "lib/log.h"
#include "client/shutdown.h"
#include "client/graphics.h"
#include "client/game.h"
#include "ui/chat.h"
#include "ui/scene_viewer.h"

client_system_manager *csm;

client_system_manager::client_system_manager()
{
  client_ui -> insert(new UI_chat(&cl, boost::bind(&client_system_manager::handle_linear_input, this, _1)));
}

client_system_manager::~client_system_manager()
{
  if(e)
  {
    delete e;
  }
}

void client_system_manager::say(std::string msg)
{ 
  BOOST_LOG_TRIVIAL(info) << msg;
  cl.add(message(ct_system, msg));
}

void client_system_manager::feed_command(std::string line)
{
  if(try_editor_handler(line))
  {
    return;
  }
  if(try_viewer_handler(line))
  {
    return;
  }
  if(e && e -> try_handle(line))
  {
    return;
  }
  if(try_exit_handler(line))
  {
    return;
  }
  say("unrecognised command " + line);
}

bool client_system_manager::try_exit_handler(std::string line)
{
  if((line == "exit") || (line == "quit"))
  {
    say("okthxbye");
    shutdown();
    return true;
  }
  return false;
}

bool client_system_manager::try_editor_handler(std::string line)
{
  char tok[256];
  if(sscanf(line.c_str(), "editor open %s", tok) == 1)
  {
    e = new editor("localhost", 7000, std::string(tok));
    say("Editor opened");
    return true;
  }
  if(line == "editor close")
  {
    if(e)
    {
      e;
      say("Editor closed");
    }
    else
    {
      say("There is no editor");
    }
    e = NULL;
    return true;
  }
  return false;
}

bool client_system_manager::try_viewer_handler(std::string line)
{
  if(line == "viewer")
  {
    client_ui -> insert(new UI_scene_viewer(&current));
    return true;
  }
  char serial_id[256];
  if(sscanf(line.c_str(), "view object %s", serial_id) == 1)
  {
    try
    {
      client_ui -> insert(new UI_object_viewer(&current, oid_t(std::string(serial_id))));
      return true;
    }
    catch(std::exception &e)
    {
    }
  }
  return false;
}

void client_system_manager::handle_linear_input(std::string line)
{
  if(std::all_of(line.begin(),line.end(),isspace))
  {
    BOOST_LOG_TRIVIAL(trace) << "empty whitespace input";
    return;
  }
  if(line[0] != '/')
  {
    BOOST_LOG_TRIVIAL(trace) << "noncommand input";
    send_message(ct_local, line);
    return;
  }
  line.erase(0, 1);
  BOOST_LOG_TRIVIAL(trace) << "command input";
  feed_command(line);
}
