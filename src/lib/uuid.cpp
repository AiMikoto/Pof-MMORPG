#include "lib/uuid.h"

boost::uuids::random_generator generator;

std::string get_uuid()
{
  return boost::lexical_cast<std::string>(generator());
}

trie_node::trie_node()
{
  for(int i = 0; i < ALPHA_SIZE; i++)
  {
    nodes[i] = NULL;
  }
  weight = 0;
  flag = 0;
}

trie_node::~trie_node()
{
  for(int i = 0; i < ALPHA_SIZE; i++)
  {
    if(nodes[i])
    {
      delete nodes[i];
    }
  }
}

bool trie_node::has(const char *path)
{
  if(*path == '\0')
  {
    return flag;
  }
  else
  {
    if(nodes[*path] == NULL)
    { // prune
      return false;
    }
    return nodes[*path] -> has(path + 1);
  }
}

void trie_node::add(const char *path)
{
  if(*path == '\0')
  {
    flag = true;
  }
  else
  {
    if(nodes[*path] == NULL)
    {
      nodes[*path] = new trie_node();
    }
    nodes[*path] -> add(path + 1);
  }
  weight++;
}

void trie_node::remove(const char *path)
{
  if(*path == '\0')
  {
    flag = false;
  }
  else
  {
    nodes[*path] -> remove(path + 1);
    if(nodes[*path] -> weight == 0)
    {
      delete nodes[*path];
      nodes[*path] = NULL;
    }
  }
  weight--;
}

trie::trie()
{
  root = new trie_node();
}

trie::~trie()
{
  delete root;
}

bool trie::has(std::string uuid)
{
  return root -> has(uuid.c_str());
}

void trie::add(std::string uuid)
{
  root -> add(uuid.c_str());
}

void trie::remove(std::string uuid)
{
  if(has(uuid))
  {  
    root -> remove(uuid.c_str());
  }
}
