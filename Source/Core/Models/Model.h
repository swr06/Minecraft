#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "../Utils/Defs.h"
#include "../BlockDatabase.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft
{
	struct __ModelVertex
	{
		glm::vec3 position;
		glm::vec2 tex_coords;
	};

	class Model
	{
	public :
		Model(BlockType type) 
		{
			const std::array<GLfloat, 8>& block_coords = BlockDatabase::GetBlockTexture(type, BlockFaceType::front);

			AddVertex(glm::vec3(0, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(1, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(1, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(0, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
			AddVertex(glm::vec3(1, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(0, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(0, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(1, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
		}

		std::vector<__ModelVertex> p_ModelVertices;

	protected :
		void AddVertex(const glm::vec3& pos, const glm::vec2& tex_coords)
		{
			p_ModelVertices.push_back({ pos, tex_coords });
		}

	private :
	};
}