#include <iostream>
#include <vector>

#include <gl/glew.h>

#include "../Utils/Defs.h"
#include "../BlockTextureManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft
{
	struct ModelVertex
	{
		glm::vec3 position;
		glm::vec2 tex_coords;
	};

	/*BlockType GenerateFlower()
	{
		int r = rand() % 4;

		switch (r)
		{
		case 0 :
			return BlockType::Flower_allium;
			break;

		case 1:
			return BlockType::Flower_orchid;
			break;

		case 2:
			return BlockType::Flower_tulip_red;
			break;

		case 3:
			return BlockType::Flower_rose;
			break;

		case 4:
			return BlockType::Flower_dandelion;
			break;
		}

		return BlockType::Flower_tulip_red;
	}*/

	class Model
	{
	public :
		Model(BlockType type) 
		{
			const std::array<GLfloat, 8>& block_coords = GetBlockTexture(type, BlockFaceType::front);

			AddVertex(glm::vec3(0, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(1, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(1, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(0, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
			AddVertex(glm::vec3(1, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(0, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(0, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(1, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
		}

		std::vector<ModelVertex> p_ModelVertices;

	protected :
		void AddVertex(const glm::vec3& pos, const glm::vec2& tex_coords)
		{
			p_ModelVertices.push_back({ pos, tex_coords });
		}
	};

	/*class GrassModel : public Model
	{
	public :
		GrassModel()
		{
			const std::array<GLfloat, 8>& block_coords = GetBlockTexture(BlockType::Model_Grass, BlockFaceType::front);

			AddVertex(glm::vec3(0, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(1, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(1, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(0, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
			AddVertex(glm::vec3(1, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(0, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(0, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(1, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
		}
	};

	class DeadbushModel : public Model
	{
	public:
		DeadbushModel()
		{
			const std::array<GLfloat, 8>& block_coords = GetBlockTexture(BlockType::Model_Deadbush, BlockFaceType::front);

			AddVertex(glm::vec3(0, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(1, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(1, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(0, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
			AddVertex(glm::vec3(1, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(0, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(0, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(1, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
		}
	};

	class FlowerModel : public Model
	{
	public:
		FlowerModel(BlockType type)
		{
			const std::array<GLfloat, 8>& block_coords = GetBlockTexture(type, BlockFaceType::front);

			AddVertex(glm::vec3(0, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(1, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(1, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(0, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
		}
	};*/
}