#pragma once
#include "scene/objects.h"
#include "components/component.h"
#include "core/constants.h"

namespace engine {
	class MeshFilter : public Component {
	public:
		uint meshID;

		MeshFilter(uint meshID);
		~MeshFilter();

		void setType();
	};
}