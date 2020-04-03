#include "client/graphics.h"
#include "client/game.h"
#include "graphics/gpu.h"
#include "lib/log.h"
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>

boost::thread *t_render;

boost::property_tree::ptree push_buffer;

boost::barrier bar(2);

bool buffer_full = false;

void gfx_init()
{
  BOOST_LOG_TRIVIAL(trace) << "Initializing GPU context";
  engine::gpu = new engine::GPU();
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
  BOOST_LOG_TRIVIAL(trace) << "attempting to change scene";
  scene_lock.lock();
  BOOST_LOG_TRIVIAL(trace) << "lock aqcuired";
  engine::gpu -> activeScenes.clear();
  if(current)
  {
    BOOST_LOG_TRIVIAL(trace) << "deleting old scene";
    delete current;
  }
  current = new engine::Scene(push_buffer);
  engine::gpu -> activeScenes.push_back(current);
  buffer_full = false;
  scene_lock.unlock();
  bar.wait();
}

void gfx_destroy()
{
  engine::gpu -> glContext -> quit = true;
  t_render -> join();
  delete t_render;
}

void gfx_duty()
{
  engine::gpu -> initializeContext();
  glfwSwapInterval(0);
  BOOST_LOG_TRIVIAL(trace) << "GPU context initialized";
  init_l.lock();
  init_l.unlock();
  BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
  while(!engine::gpu -> glContext -> quit)
  {
    if(buffer_full)
    {
      gfx_push();
    }
    scene_lock.lock();
    engine::gpu -> update();
    scene_lock.unlock();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
  }
  BOOST_LOG_TRIVIAL(trace) << "Terminating renderer";
}
