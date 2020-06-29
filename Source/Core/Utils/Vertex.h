#include <iostream>

namespace Minecraft
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
		float face_type; 
	};
}