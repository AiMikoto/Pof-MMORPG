#include "ui/utils.h"
#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "components/solid_object.h"
#include "components/phys_collider.h"

void draw_game_object(ctx_t *ctx, engine::GameObject *o, std::string path, oid_t& oid)
{
  std::string oname = "Object " + oid.serialise();
  if(nk_tree_push_hashed(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED, H_GET(path)))
  {
    std::string tpath = path + "t";
    draw_transform(ctx, o -> transform, tpath);
    std::string cpath = path + "c";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Components", NK_MINIMIZED, H_GET(cpath)))
    {
      for(int i = 0; i < o -> components.size(); i++)
      {
        std::string ccpath = cpath + std::to_string(i);
        draw_component(ctx, o -> components[i], ccpath);
      }
      nk_tree_pop(ctx);
    }
    std::string opath = path + "o";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Children", NK_MINIMIZED, H_GET(opath)))
    {
      for(auto it : o -> children)
      {
        oid.at(it.first);
        std::string oopath = path + std::to_string(it.first);
        draw_game_object(ctx, it.second, oopath, oid);
        oid.pop();
      }
      nk_tree_pop(ctx);
    }
    nk_tree_pop(ctx);
  }
}

void draw_component(ctx_t *ctx, engine::Component *c, std::string path)
{
  if(c -> name == "MeshFilter")
  {
    engine::MeshFilter *mf = static_cast<engine::MeshFilter *>(c);
    std::string oname = "Mesh filter";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED, H_GET(path)))
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
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED, H_GET(path)))
    {
      nk_layout_row_dynamic(ctx, 20, 2);
      nk_tree_pop(ctx);
    }
  }
  if(c -> name == "solid_object")
  {
    solid_object *so = static_cast<solid_object *>(c);
    std::string oname = "Solid object";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED, H_GET(path)))
    {
      nk_layout_row_dynamic(ctx, 20, 2);
      nk_label(ctx, "Mass", NK_TEXT_LEFT);
      nk_label(ctx, std::to_string(so -> m).c_str(), NK_TEXT_LEFT);
      nk_label(ctx, "Inverse Mass", NK_TEXT_LEFT);
      nk_label(ctx, std::to_string(so -> im).c_str(), NK_TEXT_LEFT);
      std::string afpath = path + "af";
      if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Accumulated force", NK_MINIMIZED, H_GET(afpath)))
      {
        draw_dvec(ctx, so -> force_acc);
        nk_tree_pop(ctx);
      }
      std::string vpath = path + "v";
      if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Velocity", NK_MINIMIZED, H_GET(vpath)))
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
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, oname.c_str(), NK_MINIMIZED, H_GET(path)))
    {
      nk_layout_row_dynamic(ctx, 20, 2);
      nk_label(ctx, "Type", NK_TEXT_LEFT);
      nk_label(ctx, "box", NK_TEXT_LEFT);
      std::string spath = path + "s";
      if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Size", NK_MINIMIZED, H_GET(spath)))
      {
        draw_dvec(ctx, pc -> size);
        nk_tree_pop(ctx);
      }
      nk_tree_pop(ctx);
    }
  }
}

void draw_transform(ctx_t *ctx, engine::Transform t, std::string path)
{
  if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Transform", NK_MINIMIZED, H_GET(path)))
  {
    std::string ppath = path + "pos";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Position", NK_MINIMIZED, H_GET(ppath)))
    {
      draw_dvec(ctx, t.position);
      nk_tree_pop(ctx);
    }
    std::string rpath = path + "rot";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Rotation", NK_MINIMIZED, H_GET(rpath)))
    {
      draw_dvec(ctx, t.rotation);
      nk_tree_pop(ctx);
    }
    std::string spath = path + "sca";
    if(nk_tree_push_hashed(ctx, NK_TREE_NODE, "Scale", NK_MINIMIZED, H_GET(spath)))
    {
      draw_dvec(ctx, t.scale);
      nk_tree_pop(ctx);
    }
    nk_tree_pop(ctx);
  }
}

void draw_dvec(ctx_t *ctx, glm::dvec3 v)
{ 
  nk_layout_row_dynamic(ctx, 20, 2);
  nk_label(ctx, "x", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.x).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "y", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.y).c_str(), NK_TEXT_LEFT);
  nk_label(ctx, "z", NK_TEXT_LEFT);
  nk_label(ctx, std::to_string(v.z).c_str(), NK_TEXT_LEFT);
}

void draw_dvec(ctx_t *ctx, glm::dvec4 v)
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

void draw_dvec(ctx_t *ctx, glm::dquat v)
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
