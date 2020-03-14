#pragma once
#include "graphics_files.h"
#include <vector>
#include <string>
#include <map>
#include "transform.h"
#include "shader.h"
#include <boost/property_tree/ptree.hpp>

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
		void addParent(llong parentID);
		void addChild(llong childID);
		void addChildren(std::vector<llong> children);
		virtual void update(GLFWwindow* window);
		void updateTransform();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	private:
		void setup();
		void generateID();		
	};
}
