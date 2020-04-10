#include "ui/scene_viewer.h"
#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "components/solid_object.h"
#include "components/phys_collider.h"

#define H_GET(x) x.c_str(), x.size(), 1

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
  if(nk_begin(ctx, "Scene Viewer", nk_rect(20, 30, 150, 600), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE))
  {
    engine::Scene *scene = *s;
    if(scene == NULL)
    {
      goto UI_scene_viewer_draw_end;
    }
    if(nk_tree_push(ctx, NK_TREE_TAB, "Objects", NK_MINIMIZED))
    {
      for(auto it : scene -> children)
      {
        draw_game_object(ctx, it.second, std::to_string(it.first));
      }
      nk_tree_pop(ctx);
    }
  }
  UI_scene_viewer_draw_end:
  nk_end(ctx);
}

void UI_scene_viewer::draw_game_object(ctx_t *ctx, engine::GameObject *o, std::string path)
{
  std::string oname = "Object";
  if(nk_tree_push(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED))
  {
    std::string tpath = path + "t";
    draw_transform(ctx, o -> transform, path);
    for(auto c : o -> components)
    {
      draw_component(ctx, c, path);
    }
    nk_tree_pop(ctx);
  }
}

void UI_scene_viewer::draw_component(ctx_t *ctx, engine::Component *c, std::string path)
{
  if(c -> name == "MeshFilter")
  {
    engine::MeshFilter *mf = static_cast<engine::MeshFilter *>(c);
    std::string oname = "Mesh filter";
    if(nk_tree_push(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED))
    {
      nk_layout_row_dynamic(ctx, 20, 2);
      nk_label(ctx, "Model path", NK_TEXT_LEFT);
      nk_label(ctx, mf -> modelPath.c_str(), NK_TEXT_LEFT);
      nk_label(ctx, "Default model path", NK_TEXT_LEFT);
      nk_label(ctx, mf -> defaultModelPath.c_str(), NK_TEXT_LEFT);
      nk_tree_pop(ctx);
    }
  }
  if(c -> name == "MeshRenderer")
  {
    engine::MeshRenderer *mr = static_cast<engine::MeshRenderer *>(c);
    std::string oname = "Mesh renderer";
    if(nk_tree_push(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED))
    {
      nk_layout_row_dynamic(ctx, 20, 2);
      nk_tree_pop(ctx);
    }
  }
  if(c -> name == "solid_object")
  {
    solid_object *so = static_cast<solid_object *>(c);
    std::string oname = "Solid object";
    if(nk_tree_push(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED))
    {
      nk_layout_row_dynamic(ctx, 20, 2);
      nk_label(ctx, "Mass", NK_TEXT_LEFT);
      nk_label(ctx, std::to_string(so -> m).c_str(), NK_TEXT_LEFT);
      nk_label(ctx, "Inverse Mass", NK_TEXT_LEFT);
      nk_label(ctx, std::to_string(so -> im).c_str(), NK_TEXT_LEFT);
      if(nk_tree_push(ctx, NK_TREE_NODE, "Accumulated force", NK_MINIMIZED))
      {
        draw_dvec(ctx, so -> force_acc);
        nk_tree_pop(ctx);
      }
      if(nk_tree_push(ctx, NK_TREE_NODE, "Velocity", NK_MINIMIZED))
      {
        draw_dvec(ctx, so -> velocity);
        nk_tree_pop(ctx);
      }
      nk_tree_pop(ctx);
    }
  }
  if(c -> name == "physical_collider")
  {
    physical_collider *pc = static_cast<physical_collider *>(c);
    std::string oname = "Physical collider";
    if(nk_tree_push(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED))
    {
      nk_layout_row_dynamic(ctx, 20, 2);
      nk_label(ctx, "Type", NK_TEXT_LEFT);
      nk_label(ctx, "box", NK_TEXT_LEFT);
      if(nk_tree_push(ctx, NK_TREE_NODE, "Size", NK_MINIMIZED))
      {
        draw_dvec(ctx, pc -> size);
        nk_tree_pop(ctx);
      }
      nk_tree_pop(ctx);
    }
  }
}

void UI_scene_viewer::draw_transform(ctx_t *ctx, engine::Transform t, std::string path)
{
  if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Transform", NK_MINIMIZED, H_GET(path)))
  {
    std::string ppath = path + "pos";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Position", NK_MINIMIZED, H_GET(ppath)))
    {
      draw_dvec(ctx, t.position);
      nk_tree_pop(ctx);
    }
    std::string rpath = path + "pos";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Rotation", NK_MINIMIZED, H_GET(rpath)))
    {
      draw_dvec(ctx, t.rotation);
      nk_tree_pop(ctx);
    }
    std::string spath = path + "pos";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Scale", NK_MINIMIZED, H_GET(spath)))
    {
      draw_dvec(ctx, t.scale);
      nk_tree_pop(ctx);
    }
    nk_tree_pop(ctx);
  }
}

void UI_scene_viewer::draw_dvec(ctx_t *ctx, glm::dvec3 v)
{ 
  nk_layout_row_dynamic(ctx, 20, 2);
  nk_label(ctx, "x", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.x).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "y", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.y).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "z", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.z).c_str(), NK_TEXT_LEFT);
}

void UI_scene_viewer::draw_dvec(ctx_t *ctx, glm::dvec4 v)
{
  nk_layout_row_dynamic(ctx, 20, 2);
  nk_label(ctx, "w", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.w).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "x", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.x).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "y", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.y).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "z", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.z).c_str(), NK_TEXT_LEFT);
}

void UI_scene_viewer::draw_dvec(ctx_t *ctx, glm::dquat v)
{
  nk_layout_row_dynamic(ctx, 20, 2);
  nk_label(ctx, "w", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.w).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "x", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.x).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "y", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.y).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "z", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.z).c_str(), NK_TEXT_LEFT);
}
