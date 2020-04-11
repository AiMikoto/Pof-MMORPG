#include "ui/ui.h"

UI_master::~UI_master()
{
  lock.lock();
  for(auto elem:initialisation_queue)
  {
    delete elem;
  }
  for(auto elem:active_elements)
  {
    delete elem;
  }
  lock.unlock();
}

void UI_master::insert(UI_element *e)
{
  lock.lock();
  initialisation_queue.insert(e);
  lock.unlock();
}

void UI_master::erase(UI_element *e)
{
  lock.lock();
  e -> suicide = true;
  lock.unlock();
}

void UI_master::init(ctx_t *ctx)
{
  lock.lock();
  auto iq = initialisation_queue;
  initialisation_queue.clear();
  lock.unlock();
  for(auto elem:iq)
  {
    elem -> init(ctx);
    active_elements.insert(elem);
  }
}

void UI_master::visit(ctx_t *ctx)
{
  lock.lock();
  auto ae = active_elements;
  lock.unlock();
  for(auto elem:ae)
  {
    elem -> visit(ctx);
  }
}

void UI_master::cleanup(ctx_t *ctx)
{
  lock.lock();
  auto ae = active_elements;
  lock.unlock();
  for(auto elem:ae)
  {
    if(elem -> suicide)
    {
      lock.lock();
      active_elements.erase(elem);
      initialisation_queue.erase(elem);
      lock.unlock();
      elem -> destroy(ctx);
      delete elem;
    }
  }
}
