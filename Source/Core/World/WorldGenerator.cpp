#include "WorldGenerator.h"

namespace Minecraft
{
    void SetVerticalBlocks(Chunk* chunk, int x, int z, int y_level)
    {
        for (int i = 0; i < y_level; i++)
        {
            if (i >= 4)
            {
                chunk->AddBlock(BlockType::Dirt, glm::vec3(x, i, z));
            }

            else
            {
                chunk->AddBlock(BlockType::Stone, glm::vec3(x, i, z));
            }
        }
    }

    void GenerateChunk(Chunk* chunk)
    {
        FastNoise myNoise;
        myNoise.SetNoiseType(FastNoise::PerlinFractal);

        glm::vec3 position;
        float noiseScale = 20.0f;

        for (int x = 0; x < ChunkSizeX; x++)
        {
            for (int z = 0; z < ChunkSizeZ; z++)
            {
                SetVerticalBlocks(chunk, x, z, (myNoise.GetNoise(x, z) / 2 + 1) * noiseScale);
            }
        }
    }
}