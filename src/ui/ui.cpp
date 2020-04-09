#include "ui/ui.h"

UI::~UI()
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

void UI::insert(UI_element *e)
{
  lock.lock();
  initialisation_queue.insert(e);
  lock.unlock();
}

void UI::erase(UI_element *e)
{
  lock.lock();
  active_elements.erase(e);
  initialisation_queue.erase(e);
  lock.unlock();
}

void UI::init(ctx_t *ctx)
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

void UI::visit(ctx_t *ctx)
{
  lock.lock();
  for(auto elem:active_elements)
  {
    elem -> visit(ctx);
  }
  lock.unlock();
}

void UI::cleanup(ctx_t *ctx)
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
