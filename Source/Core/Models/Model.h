#include <iostream>
#include <vector>

#include <gl/glew.h>

#include "../Utils/Vertex.h"
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

	BlockType GenerateFlower()
	{
		/*
		Flower_allium,
		Flower_orchid,
		Flower_tulip_red,
		Flower_rose,
		Flower_houstonia,
		Flower_dandelion,
		*/

		int r = rand() % 6;

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
			return BlockType::Flower_houstonia;
			break;

		case 5:
			return BlockType::Flower_dandelion;
			break;
		}

		return BlockType::Flower_tulip_red;
	}

	class Model
	{
	public :
		Model() {}
		std::vector<ModelVertex> p_ModelVertices;

	protected :
		void AddVertex(const glm::vec3& pos, const glm::vec2& tex_coords)
		{
			p_ModelVertices.push_back({ pos, tex_coords });
		}
	};

	class GrassModel : public Model
	{
	public :
		GrassModel()
		{
			Block b = { BlockType::Grass };
			const std::array<GLfloat, 8>& block_coords = GetModelTexture(b);

			// Add 2 diagonal planes
			// (0, 0, 0), (1, 0, 1), (1, 1, 1), (0, 1, 0)
			// (1, 0, 0), (0, 0, 1), (0, 1, 1), (1, 1, 0)

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
			Block b = { BlockType::Model_Deadbush };
			const std::array<GLfloat, 8>& block_coords = GetModelTexture(b);

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
		FlowerModel()
		{
			Block b = { GenerateFlower() };
			const std::array<GLfloat, 8>& block_coords = GetModelTexture(b);

			AddVertex(glm::vec3(0, 0, 0), glm::vec2(block_coords[0], block_coords[1]));
			AddVertex(glm::vec3(1, 0, 1), glm::vec2(block_coords[2], block_coords[3]));
			AddVertex(glm::vec3(1, 1, 1), glm::vec2(block_coords[4], block_coords[5]));
			AddVertex(glm::vec3(0, 1, 0), glm::vec2(block_coords[6], block_coords[7]));
		}
	};
}