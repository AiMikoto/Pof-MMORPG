#include "client/graphics.h"
#include "client/game.h"
#include "graphics/gpu.h"
#include "lib/log.h"
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include "client/shutdown.h"
#include "ui/console.h"

boost::thread *t_render;

boost::property_tree::ptree push_buffer;

boost::barrier bar(2);

bool buffer_full = false;

UI_master *client_ui;

void gfx_init()
{
  BOOST_LOG_TRIVIAL(trace) << "Initializing GPU context";
  engine::gpu = new engine::GPU();
  BOOST_LOG_TRIVIAL(trace) << "Creating UI";
  client_ui = new UI_master();
  client_ui -> insert(new UI_console(handle_linear_input));
  BOOST_LOG_TRIVIAL(trace) << "starting render thread";
  t_render = new boost::thread(gfx_duty);
}

void gfx_buffer(boost::property_tree::ptree node)
{
  scene_lock.lock();
  push_buffer = node;
  buffer_full = true;
  scene_lock.unlock();
  BOOST_LOG_TRIVIAL(trace) << "waiting for scene update";
  bar.wait();
  BOOST_LOG_TRIVIAL(trace) << "scene updated";
}

void gfx_push()
{
  std::string old_tag = "";
  BOOST_LOG_TRIVIAL(trace) << "attempting to change scene";
  scene_lock.lock();
  BOOST_LOG_TRIVIAL(trace) << "lock aqcuired";
  engine::gpu -> activeScenes.clear();
  if(current)
  {
    BOOST_LOG_TRIVIAL(trace) << "deleting old scene";
    old_tag = current -> tag;
    delete current;
  }
  current = new engine::Scene(push_buffer);
  engine::gpu -> activeScenes.push_back(current);
  buffer_full = false;
  scene_lock.unlock();
  apply_slice_buffer();
  if(old_tag != "")
  {
    wipe(old_tag);
  }
  bar.wait();
}

boost::barrier gpu_destroyer_bar(2);

void gfx_destroy()
{
  engine::gpu -> glContext -> quit = true;
  gpu_destroyer_bar.wait();
  t_render -> join();
  delete t_render;
}

void gfx_duty()
{
  engine::gpu -> initializeContext();
  glfwSwapInterval(0);
  BOOST_LOG_TRIVIAL(trace) << "GPU context initialized";
  engine::gpu -> addUI(client_ui);
  BOOST_LOG_TRIVIAL(trace) << "UI attached";
  init_l.lock();
  init_l.unlock();
  BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
  while(!engine::gpu -> glContext -> quit)
  {
    if(buffer_full)
    {
      gfx_push();
    }
    apply_slice_buffer();
    scene_lock.lock();
    engine::gpu -> update();
    scene_lock.unlock();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
  }
  shutdown();
  BOOST_LOG_TRIVIAL(trace) << "Terminating renderer";
  gpu_destroyer_bar.wait();
  delete engine::gpu;
}
