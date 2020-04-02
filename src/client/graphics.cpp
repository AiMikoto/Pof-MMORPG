#include "client/graphics.h"
#include "client/game.h"
#include "graphics/gpu.h"
#include "lib/log.h"
#include <boost/thread.hpp>

boost::thread *t_render;

void gfx_init()
{
  BOOST_LOG_TRIVIAL(trace) << "Initializing GPU context";
  engine::gpu = new engine::GPU();
  BOOST_LOG_TRIVIAL(trace) << "pushing empty scene";
  gfx_push(new engine::Scene());
  BOOST_LOG_TRIVIAL(trace) << "starting render thread";
  t_render = new boost::thread(gfx_duty);
}

void gfx_push(engine::Scene *s)
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
  engine::gpu -> activeScenes.push_back(s);
  current = s;
  scene_lock.unlock();
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
    scene_lock.lock();
    engine::gpu -> update();
    scene_lock.unlock();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
  }
  BOOST_LOG_TRIVIAL(trace) << "Terminating renderer";
  delete engine::gpu;
}
