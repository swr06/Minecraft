#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

#include "../../Block.h"
#include "../../Utils/Defs.h"

// The various structures found in the world

namespace Minecraft
{
	enum WorldStructureType
	{
		Trees,
		Cacti,
		Undefined
	};

	class WorldStructure
	{
	public :

		WorldStructure()
		{
			Block b;

			p_Structure = new std::array<std::array<std::array<Block, MaxStructureX>, MaxStructureY>, MaxStructureZ>;
			p_StructureType = WorldStructureType::Undefined;

			for (int i = 0; i < MaxStructureX; i++)
			{
				for (int j = 0; j < MaxStructureY; j++)
				{
					for (int k = 0; k < MaxStructureZ; k++)
					{
						b.p_BlockType = BlockType::Air;
						b.p_Position = glm::vec3(i, j, k);

						p_Structure->at(i).at(j).at(k) = b;
					}
				}
			}
		}

		~WorldStructure()
		{
			delete p_Structure;
		}

		WorldStructureType p_StructureType;
		std::array<std::array<std::array<Block, MaxStructureX>, MaxStructureY>, MaxStructureZ>* p_Structure;

	protected : 

		void SetBlock(int x, int y, int z, BlockType type)
		{
			Block b;

			b.p_Position = glm::vec3(x, y, z);
			b.p_BlockType = type;
			p_Structure->at(x).at(y).at(z) = b;
		}
	};

	class TreeStructure : public WorldStructure
	{
	public :

		TreeStructure()
		{
			// Define the tree structure

			// Add the Bark
			SetBlock(2, 0, 2, BlockType::Wood);
			SetBlock(2, 1, 2, BlockType::Wood);
			SetBlock(2, 2, 2, BlockType::Wood);
			SetBlock(2, 3, 2, BlockType::Leaf);

			SetBlock(0, 3, 0, BlockType::Leaf);
			SetBlock(0, 3, 1, BlockType::Leaf);
			SetBlock(0, 3, 2, BlockType::Leaf);
			SetBlock(0, 3, 3, BlockType::Leaf);

			SetBlock(1, 3, 0, BlockType::Leaf);
			SetBlock(1, 3, 1, BlockType::Leaf);
			SetBlock(1, 3, 2, BlockType::Leaf);
			SetBlock(1, 3, 3, BlockType::Leaf);

			SetBlock(2, 3, 0, BlockType::Leaf);
			SetBlock(2, 3, 1, BlockType::Leaf);
			SetBlock(2, 3, 2, BlockType::Leaf);
			SetBlock(2, 3, 3, BlockType::Leaf);

			SetBlock(3, 3, 0, BlockType::Leaf);
			SetBlock(3, 3, 1, BlockType::Leaf);
			SetBlock(3, 3, 2, BlockType::Leaf);
			SetBlock(3, 3, 3, BlockType::Leaf);
		}
	};
}