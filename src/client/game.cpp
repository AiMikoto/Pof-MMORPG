#include "client/game.h"
#include "client/client.h"
#include "client/graphics.h"
#include "lib/log.h"
#include "client/editor.h"
#include "ui/chat.h"
#include "client/shutdown.h"

user_card_library ucl;
engine::Scene *current = NULL;
std::mutex scene_lock;
chat_log cl;
editor *e = NULL;
client_system_manager *csm;

std::map<std::string, std::map<long long, slice_t>> slices;

bool try_exit_handler(std::string line)
{
  if((line == "exit") || (line == "quit"))
  {
    csm -> say("okthxbye");
    shutdown();
    return true;
  }
  return false;
}

bool try_editor_handler(std::string line)
{
  char tok[256];
  if(sscanf(line.c_str(), "editor open %s", tok) == 1)
  {
    e = new editor("localhost", 7000, std::string(tok));
    csm -> say("Editor opened");
    return true;
  }
  if(line == "editor close")
  {
    if(e)
    {
      delete e;
      csm -> say("Editor closed");
    }
    {
      csm -> say("There is no editor");
    }
    e = NULL;
    return true;
  }
  return false;
}

void handle_linear_input(std::string line)
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
  if(try_editor_handler(line))
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
  csm -> say("unrecognised command " + line);
}

void game_init()
{
  client_ui -> insert(new UI_chat(&cl, handle_linear_input));
  csm = new client_system_manager(&cl);
}

void game_destroy()
{
  if(e)
  {
    delete e;
    e = NULL;
  }
  delete csm;
}

void move(std::string host, int port)
{
  init_l.lock();
  init_l.unlock();
  // TODO: put up loading screen
  BOOST_LOG_TRIVIAL(trace) << "closing current instance";
  delete current_instance;
  if(current)
  {
    wipe(current -> tag);
  }
  // connect to new instance
  BOOST_LOG_TRIVIAL(trace) << "connecting to new instance";
  current_instance = instance_builder(host, port);
  BOOST_LOG_TRIVIAL(trace) << "attempting to log-in";
  std::string tok = ucl.get(username).tree().get<std::string>("user.token");
  current_instance -> authenticate_token(username, tok);
  BOOST_LOG_TRIVIAL(trace) << "finished instance movement";
}

void set_scene(boost::property_tree::ptree node)
{
  BOOST_LOG_TRIVIAL(trace) << "received scene " << node.get<long long>("generation") << ":" << node.get<std::string>("tag");
  gfx_buffer(node);
  // TODO: take down loading screen
}

void add_slice(slice_t next_slice)
{
  BOOST_LOG_TRIVIAL(trace) << "received slice " << next_slice.tag << ":" << next_slice.target_generation;
  slices[next_slice.tag][next_slice.origin_generation] = next_slice;
}

void apply_slice_buffer()
{
  scene_lock.lock();
  while((current) && (slices.find(current -> tag) != slices.end()) && (slices[current -> tag].find(current -> generation) != slices[current -> tag].end()))
  {
    BOOST_LOG_TRIVIAL(trace) << "applying slice " << slices[current -> tag][current -> generation].tag << ":" << slices[current -> tag][current -> generation].target_generation;
    apply_slice(current, slices[current -> tag][current -> generation]);
  }
  scene_lock.unlock();
}

void wipe(std::string tag)
{
  slices[tag].clear();
}

void send_message(chat_target target, std::string payload)
{
  message m(target, payload);
  BOOST_LOG_TRIVIAL(trace) << "issuing message " << m.uuid;
  call c;
  c.tree().put(OPCODE, OP_IRC);
  c.tree().put_child("payload", m.encode());
  current_instance -> safe_write(c);
}
