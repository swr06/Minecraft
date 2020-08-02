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

	class LightRemovalNode
	{
	public : 
		LightRemovalNode(const glm::vec3& position, int light, Chunk* chunk) : p_Position(position), p_LightValue(light), p_Chunk(chunk)
		{
			
		}

		glm::vec3 p_Position;
		uint8_t p_LightValue;
		Chunk* p_Chunk;
	};
}