#pragma once
#include "graphics_files.h"
#include <vector>
#include <string>
#include <map>
#include "transform.h"
#include "shader.h"

namespace graphics {

	class GameObject {
	public:
		std::string name, tag;
		llong id = -1;
		llong parentID = -1;
		std::vector<llong> childrenIDs;
		Transform transform;
		uint type;
		bool mUpdate = false;

		GameObject();
		~GameObject();
		GameObject(llong parentID);
		GameObject(llong id, llong parentID);
		GameObject(std::vector<llong> childrenIDs);
		GameObject(llong parentID, std::vector<llong> childrenIDs);
		GameObject(llong id, llong parentID, std::vector<llong> childrenIDs);
		void add_child(llong child);
		void add_children(std::vector<llong> children);
		virtual void update(GLFWwindow* window);
		void updateTransform();
	private:
		void setup();
		void generateID();		
	};
}
