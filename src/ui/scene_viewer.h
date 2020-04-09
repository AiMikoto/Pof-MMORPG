#ifndef UI_SCENE_VIEWER_H
#define UI_SCENE_VIEWER_H

#include "ui/ui.h"
#include "scene/scene.h"

class UI_scene_viewer : public UI_element
{
public:
  UI_scene_viewer(engine::Scene **s);
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  void draw(ctx_t *ctx);
  engine::Scene **s;
};

#endif // UI_SCENE_VIEWER_H
