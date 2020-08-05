#include "WorldGenerator.h"

namespace Minecraft
{
    // Sets the vertical blocks based on the biome
    void SetVerticalBlocks(Chunk* chunk, int x, int z, int y_level, Biome biome)
    {
        if (y_level >= CHUNK_SIZE_Y)
        {
            y_level = CHUNK_SIZE_Y - 1;
        }

        for (int i = 0; i < y_level; i++)
        {
            if (biome == Biome::Grassland)
            {
                if (i >= y_level - 1)
                {
                    chunk->SetBlock(BlockType::Grass, glm::vec3(x, i, z));
                }

                else if (i >= y_level - 5)
                {
                    chunk->SetBlock(BlockType::Dirt, glm::vec3(x, i, z));
                }

                else
                {
                    chunk->SetBlock(BlockType::Stone, glm::vec3(x, i, z));
                }
            }

            else if (biome == Biome::Desert)
            {
                if (i >= y_level - 6)
                {
                    chunk->SetBlock(BlockType::Sand, glm::vec3(x, i, z));
                }

                else
                {
                    chunk->SetBlock(BlockType::Stone, glm::vec3(x, i, z));
                }
            }
        }
    }

    void FillInWorldStructure(Chunk* chunk, WorldStructure* structure, int x, int y, int z)
    {
        Block block;

        for (int i = x, sx = 0; i < x + MAX_STRUCTURE_X; i++, sx++)
        {
            for (int j = y, sy = 0; j < y + MAX_STRUCTURE_Y; j++, sy++)
            {
                for (int k = z, sz = 0; k < z + MAX_STRUCTURE_Z; k++, sz++)
                {
                    if (i < CHUNK_SIZE_X && j < CHUNK_SIZE_Y && k < CHUNK_SIZE_Z && sx < MAX_STRUCTURE_X && sy < MAX_STRUCTURE_Y && sz < MAX_STRUCTURE_Z)
                    {
                        if (structure->p_Structure->at(sx).at(sy).at(sz).p_BlockType != BlockType::Air)
                        {
                            chunk->SetBlock(static_cast<BlockType>(structure->p_Structure->at(sx).at(sy).at(sz).p_BlockType), glm::vec3(i, j, k));
                        }
                    }
                }
            }
        }
    }

    Biome GetBiome(float chunk_noise)
    {
        // Quantize the noise into various levels and frequency

        const float grass_land = 0.1f;
        const float desert = 0.6f;
        const float ocean = 0.6f;

        if (chunk_noise <= 0)
        {
            return Biome::Grassland;
        }

        else if (chunk_noise > 0)
        {
            return Biome::Desert;
        }
    }

    void GenerateChunk(Chunk* chunk, const int WorldSeed)
    {
        static Random SeedEngine;
        static Random WorldTreeGenerator(WorldSeed);
        static FastNoise WorldGenerator(WorldSeed);
        static FastNoise WorldGeneratorMultiply_1(WorldSeed);
        static FastNoise BiomeGenerator(WorldSeed);

        // Set the chunk state
        chunk->p_ChunkState = ChunkState::Generated;
        
        WorldGenerator.SetNoiseType(FastNoise::Simplex);
        BiomeGenerator.SetNoiseType(FastNoise::Simplex);

        float generated_x = 0;
        float generated_y = 0;
        float generated_z = 0;

        static TreeStructure WorldStructureTree;
        static CactusStructure WorldStructureCactus;
        WorldStructure* Structure;

        static float HeightMap[CHUNK_SIZE_X][CHUNK_SIZE_Y]; // 2D heightmap to create terrain

        Biome chunk_biome;

        chunk_biome = GetBiome(BiomeGenerator.GetNoise(chunk->p_Position.x * CHUNK_SIZE_X, chunk->p_Position.y * CHUNK_SIZE_Y, chunk->p_Position.z * CHUNK_SIZE_Z));

        for (int x = 0; x < CHUNK_SIZE_X; x++)
        {
            for (int y = 0; y < CHUNK_SIZE_Y; y++)
            {
                HeightMap[x][y] = WorldGenerator.GetNoise(x + chunk->p_Position.x * CHUNK_SIZE_X, y + chunk->p_Position.z * CHUNK_SIZE_Z) *
                    WorldGeneratorMultiply_1.GetNoise((x + chunk->p_Position.x * CHUNK_SIZE_X) + 4, (y + chunk->p_Position.z * CHUNK_SIZE_Z) + 8) ;
            }
        }

        for (int x = 0; x < CHUNK_SIZE_X; x++)
        {
            for (int z = 0; z < CHUNK_SIZE_Z; z++)
            {
                generated_x = x;
                generated_z = z;

                switch (chunk_biome)
                {
                    case Biome::Grassland :
                    {
                        generated_y = (HeightMap[x][z] / 2 + 1.0) * (CHUNK_SIZE_Y - 32);
                        Structure = &WorldStructureTree;
                        break;
                    }

                    case Biome::Desert : 
                    {
                        generated_y = (HeightMap[x][z] / 2 + 1.0) * (CHUNK_SIZE_Y - 32);
                        Structure = &WorldStructureCactus;
                        break;
                    }

                    default :
                    {
                        generated_y = (HeightMap[x][z] / 2 + 1.0) * (CHUNK_SIZE_Y - 32);
                        Structure = nullptr;
                        break;
                    }
                }

                SetVerticalBlocks(chunk, generated_x, generated_z, generated_y, chunk_biome);
                
                if (WorldTreeGenerator.UnsignedInt(75) == 7 &&
                    generated_x + MAX_STRUCTURE_X < CHUNK_SIZE_X && 
                    generated_y + MAX_STRUCTURE_Y < CHUNK_SIZE_Y &&
                    generated_z + MAX_STRUCTURE_Z < CHUNK_SIZE_Z &&
                    Structure != nullptr)
                {
                     FillInWorldStructure(chunk, Structure, generated_x, generated_y - 1, generated_z);
                }
            }
        }
    }
}