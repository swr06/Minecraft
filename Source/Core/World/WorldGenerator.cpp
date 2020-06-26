#include "WorldGenerator.h"

namespace Minecraft
{
	void GenerateChunk(Chunk* chunk)
	{
		for (int x = 0; x < ChunkSizeX; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				for (int z = 0; z < ChunkSizeZ; z++)
				{
					if (y <= 4)
					{
						chunk->AddBlock(BlockType::Dirt, glm::vec3((float)x, (float)y, (float)z));
					}

					else
					{
						chunk->AddBlock(BlockType::Stone, glm::vec3((float)x, (float)y, (float)z));
					}
				}
			}
		}
	}
}