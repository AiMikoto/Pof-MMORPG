#ifndef UI_UI_H
#define UI_UI_H

#include "lib/nuklear.h"
#include <mutex>
#include <set>

#define ctx_t struct nk_context

class UI_element
{
public:
  virtual void init(ctx_t *ctx) = 0;
  virtual void visit(ctx_t *ctx) = 0;
};

class UI
{
public:
  ~UI();
  void insert(UI_element *e);
  void erase(UI_element *e);
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  std::set<UI_element *> active_elements;
  std::set<UI_element *> initialisation_queue;
  std::mutex lock;
};

#endif
