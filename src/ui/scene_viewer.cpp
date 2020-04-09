#include "ui/scene_viewer.h"

UI_scene_viewer::UI_scene_viewer(engine::Scene **s)
{
  this -> s = s;
}

void UI_scene_viewer::init(ctx_t *ctx)
{
}

void UI_scene_viewer::visit(ctx_t *ctx)
{
  draw(ctx);
}

void UI_scene_viewer::draw(ctx_t *ctx)
{
  if(nk_begin(ctx, "SceneViewer", nk_rect(20, 30, 150, 600), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE))
  {
  }
  nk_end(ctx);
}
