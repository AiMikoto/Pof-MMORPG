#ifndef LIB_UUID_H
#define LIB_UUID_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

#define ALPHA_SIZE 256

extern boost::uuids::random_generator generator;

std::string get_uuid();

class trie_node
{
public:
  trie_node();
  ~trie_node();
  bool has(const char *path);
  void add(const char *path);
  void remove(const char *path);
private:
  struct trie_node *nodes[ALPHA_SIZE];
  int weight;
  bool flag;
};

class trie
{
public:
  trie();
  ~trie();
  bool has(std::string uuid);
  void add(std::string uuid);
  void remove(std::string uuid);
private:
  struct trie_node *root;
};

#endif
