#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>
#include "graphics/constants.h"
#include "graphics/variables.h"
#include "graphics/transform.h"

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
		size_t generateID();
	};

	
}
