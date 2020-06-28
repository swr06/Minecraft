#include "WorldGenerator.h"

namespace Minecraft
{
    void SetVerticalBlocks(Chunk* chunk, BlockType type, int x, int z, int y_level)
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
        float generated_pos[3];

        float generated_x = 0;
        float generated_y = 0;
        float generated_z = 0;

        float HeightMap[16][16]; // 2D heightmap to create terrain

        for (int x = 0; x < ChunkSizeX; x++)
        {
            for (int y = 0; y < ChunkSizeY; y++)
            {
                HeightMap[x][y] = myNoise.GetNoise(x + chunk->p_Position.x * ChunkSizeX, y + chunk->p_Position.z * ChunkSizeZ);
            }
        }


        for (int x = 0; x < ChunkSizeX; x++)
        {
            for (int y = 0; y < ChunkSizeY; y++)
            {
                for (int z = 0; z < ChunkSizeZ; z++)
                {
                    generated_x = x;
                    generated_y = (HeightMap[x][z] / 2 + 1) * ChunkSizeY;
                    generated_z = z;

                    SetVerticalBlocks(chunk, BlockType::Dirt, generated_x, generated_z, generated_y);
                }
            }
        }
    }
}