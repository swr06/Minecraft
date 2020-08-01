#include <iostream>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft
{
	class Chunk;

	class LightNode
	{
	public :

		LightNode(const glm::vec3& position, Chunk* chunk) : p_Position(position), p_Chunk(chunk)
		{

		}

		glm::vec3 p_Position;
		Chunk* p_Chunk;
	};
}