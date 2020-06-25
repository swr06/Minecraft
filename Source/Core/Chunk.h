#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "Block.h"

namespace Minecraft
{
	class Chunk
	{
	public : 
		void AddBlock(BlockType type, glm::vec3 position)
		{
			Block b;
			b.p_Position = position;
			b.p_Chunk = this;
			b.p_BlockType = type;

			m_ChunkContents[position.x][position.y][position.z] = b;
		}

	private :
		// each chunk will be a 16x16x16 block space. A 3D array

		std::array<std::array<std::array<Block, 16>, 16>, 16> m_ChunkContents;
	};
}