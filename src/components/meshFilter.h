#pragma once
#include "scene/objects.h"
#include "components/component.h"
#include "core/constants.h"
#include <boost/property_tree/ptree.hpp>

namespace engine {
	class MeshFilter : public Component {
	public:
		std::string modelPath, defaultModelPath;

		MeshFilter(std::string path);
		MeshFilter(const MeshFilter& meshFilter);
		MeshFilter(boost::property_tree::ptree node);
		~MeshFilter();

		void setup();
		void assertModel();
		boost::property_tree::ptree serialize();
		bool hasModel();
		std::vector<glm::dvec3> computeAABB();
	protected:
		bool aabbComputed;
		std::vector<glm::dvec3> aabbPoints;
		void setType();
	};
}
