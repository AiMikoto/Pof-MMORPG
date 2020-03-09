#pragma once
#include "graphics_files.h"
#include <vector>
#include <string>
#include <map>
#include "constants.h"
#include "variables.h"
#include "transform.h"

namespace graphics {

	class GameObject {
	public:
		std::string name, tag;
		size_t id;
		GameObject* parent;
		std::vector<GameObject*> children;
		Transform transform;

		GameObject();
		~GameObject();
		GameObject(GameObject* parent);
		GameObject(std::vector<GameObject*> children);
		GameObject(GameObject* parent, std::vector<GameObject*> children);
		void add_child(GameObject* child);
		void add_children(std::vector<GameObject*> children);
	private:
		void setup();
		size_t generateID();
	};

	std::map<size_t, GameObject*> gameObject;
}
