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
  active_elements.erase(e);
  initialisation_queue.erase(e);
  lock.unlock();
}

void UI_master::init(ctx_t *ctx)
{
  lock.lock();
  for(auto elem:initialisation_queue)
  {
    elem -> init(ctx);
    active_elements.insert(elem);
  }
  initialisation_queue.clear();
  lock.unlock();
}

void UI_master::visit(ctx_t *ctx)
{
  lock.lock();
  for(auto elem:active_elements)
  {
    elem -> visit(ctx);
  }
  lock.unlock();
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
      erase(elem);
    }
  }
}
