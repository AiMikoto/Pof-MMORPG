#ifndef UI_SCENE_VIEWER_H
#define UI_SCENE_VIEWER_H

#include "ui/ui.h"
#include "ui/utils.h"

class UI_object_viewer : public UI_element
{
public:
  UI_object_viewer(engine::Scene **s, oid_t oid);
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  void draw(ctx_t *ctx);
  engine::Scene **s;
  oid_t oid;
  std::string uuid;
};

class UI_scene_viewer : public UI_element
{
public:
  UI_scene_viewer(engine::Scene **s);
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  void draw(ctx_t *ctx);
  engine::Scene **s;
  std::string uuid;
};

#endif // UI_SCENE_VIEWER_H
