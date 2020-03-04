#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>

namespace graphics {
	struct GameObject {
		std::string name, tag;
		size_t id;
		GameObject* parent;
		std::vector<GameObject*> children;

		GameObject();
		~GameObject();
		GameObject(GameObject* parent);
		GameObject(std::vector<GameObject*> children);
		GameObject(GameObject* parent, std::vector<GameObject*> children);
		void add_child(GameObject* child);
		void add_children(std::vector<GameObject*> children);
		size_t generateID();
	};

	struct Transform:GameObject {
		glm::vec3 position, scale;
		glm::quat rotation;

		Transform();
		~Transform();
		Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
		glm::vec3 forward();
		glm::vec3 right();
		glm::vec3 up();
	};

	struct Camera :GameObject {
		Camera();
		~Camera();
		Camera(Transform* transform);
	};
}
