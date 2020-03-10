#pragma once
#include "graphics_files.h"
#include <vector>
#include <string>
#include <map>
#include "constants.h"
#include "variables.h"
#include "transform.h"
#include "shader.h"

namespace graphics {

	class GameObject {
	public:
		std::string name, tag;
		llong id, parentID;
		std::vector<llong> childrenIDs;
		Transform transform;
		bool update;
		uint type;

		GameObject();
		~GameObject();
		GameObject(llong parentID);
		GameObject(std::vector<llong> childrenIDs);
		GameObject(llong parentID, std::vector<llong> childrenIDs);
		void add_child(llong child);
		void add_children(std::vector<llong> children);
		virtual void draw(Shader* shader, GameObject* camera, GLFWwindow* window);
		void updateTransform();
	private:
		void setup();
		void generateID();
	};

	extern std::map<llong, GameObject*> gameObjects;
}
