#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft
{
	struct AABB
	{
		float x;
		float y;
		float z;
		float width;
		float height;
		float depth;
	};
}