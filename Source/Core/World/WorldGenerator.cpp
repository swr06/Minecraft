#include "WorldGenerator.h"

namespace Minecraft
{
	void GenerateChunk(Chunk* chunk)
	{
		FastNoise myNoise;
		myNoise.SetNoiseType(FastNoise::Perlin);

		glm::vec3 position;
		float generated_pos[3];

		float generated_x;
		float generated_y;
		float generated_z;

		for (int x = 0; x < ChunkSizeX; x++)
		{
			for (int y = 0; y < ChunkSizeY; y++)
			{
				for (int z = 0; z < ChunkSizeZ; z++)
				{
					generated_x = x;
					generated_y = ceil(myNoise.GetNoise(x, y, z) * 100);
					generated_z = z;

					if (y >= 4)
					{
						chunk->AddBlock(BlockType::Dirt, glm::vec3((int)generated_x, (int)generated_y, (int)generated_z));
					}

					else
					{
						chunk->AddBlock(BlockType::Stone, glm::vec3((int)generated_x, (int)generated_y, (int)generated_z));
					}
				}
			}
		}
	}
}