#include "ui/scene_viewer.h"

UI_scene_viewer::UI_scene_viewer(engine::Scene **s)
{
  this -> s = s;
}

void UI_scene_viewer::init(ctx_t *ctx)
{
  draw(ctx);
}

void UI_scene_viewer::visit(ctx_t *ctx)
{
  draw(ctx);
}

void UI_scene_viewer::draw(ctx_t *ctx)
{
}
