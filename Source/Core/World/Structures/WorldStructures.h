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
			p_Structure = new std::array<std::array<std::array<Block, MAX_STRUCTURE_X>, MAX_STRUCTURE_Y>, MAX_STRUCTURE_Z>;
			p_StructureType = WorldStructureType::Undefined;

			memset(p_Structure, BlockType::Air, MAX_STRUCTURE_X * MAX_STRUCTURE_Y * MAX_STRUCTURE_Z);
		}

		~WorldStructure()
		{
			delete p_Structure;
		}

		WorldStructureType p_StructureType;
		std::array<std::array<std::array<Block, MAX_STRUCTURE_X>, MAX_STRUCTURE_Y>, MAX_STRUCTURE_Z>* p_Structure;

	protected : 

		void SetBlock(int x, int y, int z, BlockType type)
		{
			Block b;

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
			SetBlock(2, 0, 2, BlockType::OakLog);
			SetBlock(2, 1, 2, BlockType::OakLog);
			SetBlock(2, 2, 2, BlockType::OakLog);
			SetBlock(2, 3, 2, BlockType::OakLog);
			SetBlock(2, 4, 2, BlockType::OakLog);

			SetBlock(2, 3, 2, BlockType::OakLeaves);

			SetBlock(0, 2, 0, BlockType::OakLeaves);
			SetBlock(0, 2, 1, BlockType::OakLeaves);
			SetBlock(0, 2, 2, BlockType::OakLeaves);
			SetBlock(0, 2, 3, BlockType::OakLeaves);
			SetBlock(1, 2, 0, BlockType::OakLeaves);
			SetBlock(1, 2, 1, BlockType::OakLeaves);
			SetBlock(1, 2, 2, BlockType::OakLeaves);
			SetBlock(1, 2, 3, BlockType::OakLeaves);
			SetBlock(2, 2, 0, BlockType::OakLeaves);
			SetBlock(2, 2, 1, BlockType::OakLeaves);
			SetBlock(2, 2, 3, BlockType::OakLeaves);
			SetBlock(3, 2, 0, BlockType::OakLeaves);
			SetBlock(3, 2, 1, BlockType::OakLeaves);
			SetBlock(3, 2, 2, BlockType::OakLeaves);
			SetBlock(3, 2, 3, BlockType::OakLeaves);
			SetBlock(4, 2, 0, BlockType::OakLeaves);
			SetBlock(4, 2, 1, BlockType::OakLeaves);
			SetBlock(4, 2, 2, BlockType::OakLeaves);
			SetBlock(4, 2, 3, BlockType::OakLeaves);

			SetBlock(0, 3, 0, BlockType::OakLeaves);
			SetBlock(0, 3, 1, BlockType::OakLeaves);
			SetBlock(0, 3, 2, BlockType::OakLeaves);
			SetBlock(0, 3, 3, BlockType::OakLeaves);
			SetBlock(1, 3, 0, BlockType::OakLeaves);
			SetBlock(1, 3, 1, BlockType::OakLeaves);
			SetBlock(1, 3, 2, BlockType::OakLeaves);
			SetBlock(1, 3, 3, BlockType::OakLeaves);
			SetBlock(2, 3, 0, BlockType::OakLeaves);
			SetBlock(2, 3, 1, BlockType::OakLeaves);
			SetBlock(2, 3, 3, BlockType::OakLeaves);
			SetBlock(3, 3, 0, BlockType::OakLeaves);
			SetBlock(3, 3, 1, BlockType::OakLeaves);
			SetBlock(3, 3, 2, BlockType::OakLeaves);
			SetBlock(3, 3, 3, BlockType::OakLeaves);
			SetBlock(4, 3, 0, BlockType::OakLeaves);
			SetBlock(4, 3, 1, BlockType::OakLeaves);
			SetBlock(4, 3, 2, BlockType::OakLeaves);
			SetBlock(4, 3, 3, BlockType::OakLeaves);

			SetBlock(0, 4, 0, BlockType::OakLeaves);
			SetBlock(0, 4, 1, BlockType::OakLeaves);
			SetBlock(0, 4, 2, BlockType::OakLeaves);
			SetBlock(0, 4, 3, BlockType::OakLeaves);
			SetBlock(1, 4, 0, BlockType::OakLeaves);
			SetBlock(1, 4, 1, BlockType::OakLeaves);
			SetBlock(1, 4, 2, BlockType::OakLeaves);
			SetBlock(1, 4, 3, BlockType::OakLeaves);
			SetBlock(2, 4, 0, BlockType::OakLeaves);
			SetBlock(2, 4, 1, BlockType::OakLeaves);	
			SetBlock(2, 4, 3, BlockType::OakLeaves);
			SetBlock(3, 4, 0, BlockType::OakLeaves);
			SetBlock(3, 4, 1, BlockType::OakLeaves);
			SetBlock(3, 4, 2, BlockType::OakLeaves);
			SetBlock(3, 4, 3, BlockType::OakLeaves);
			SetBlock(4, 4, 0, BlockType::OakLeaves);
			SetBlock(4, 4, 1, BlockType::OakLeaves);
			SetBlock(4, 4, 2, BlockType::OakLeaves);
			SetBlock(4, 4, 3, BlockType::OakLeaves);

			SetBlock(0, 5, 0, BlockType::OakLeaves);
			SetBlock(0, 5, 1, BlockType::OakLeaves);
			SetBlock(0, 5, 2, BlockType::OakLeaves);
			SetBlock(0, 5, 3, BlockType::OakLeaves);
			SetBlock(1, 5, 0, BlockType::OakLeaves);
			SetBlock(1, 5, 1, BlockType::OakLeaves);
			SetBlock(1, 5, 2, BlockType::OakLeaves);
			SetBlock(1, 5, 3, BlockType::OakLeaves);
			SetBlock(2, 5, 0, BlockType::OakLeaves);
			SetBlock(2, 5, 1, BlockType::OakLeaves);
			SetBlock(2, 5, 2, BlockType::OakLeaves);
			SetBlock(2, 5, 3, BlockType::OakLeaves);
			SetBlock(3, 5, 0, BlockType::OakLeaves);
			SetBlock(3, 5, 1, BlockType::OakLeaves);
			SetBlock(3, 5, 2, BlockType::OakLeaves);
			SetBlock(3, 5, 3, BlockType::OakLeaves);
			SetBlock(4, 5, 0, BlockType::OakLeaves);
			SetBlock(4, 5, 1, BlockType::OakLeaves);
			SetBlock(4, 5, 2, BlockType::OakLeaves);
			SetBlock(4, 5, 3, BlockType::OakLeaves);
		}
	};

	class CactusStructure : public WorldStructure
	{
	public : 
		CactusStructure()
		{
			SetBlock(2, 0, 2, BlockType::Cactus);
			SetBlock(2, 1, 2, BlockType::Cactus);
			SetBlock(2, 2, 2, BlockType::Cactus);
			SetBlock(2, 3, 2, BlockType::Cactus);
			SetBlock(2, 4, 2, BlockType::Cactus);
			SetBlock(2, 5, 2, BlockType::Cactus);
		}
	};
}