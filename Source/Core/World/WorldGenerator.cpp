#include "WorldGenerator.h"

namespace Minecraft
{
    static FastNoise BiomeGenerator(1234);
    Biome GetBiome(float chunk_noise);

    // Water levels
    constexpr int water_min = 18;
    constexpr int water_max = 90;

    // Sets the vertical blocks based on the biome
    // Also it returns the biome for the block column
    Biome SetVerticalBlocks(Chunk* chunk, int x, int z, int y_level, float real_x, float real_z)
    {
        BiomeGenerator.SetNoiseType(FastNoise::Simplex);

        float column_noise = BiomeGenerator.GetNoise(real_x, real_z);
        Biome biome = GetBiome(column_noise);
       
        if (y_level >= 128)
        {
            y_level = 128 - 1;
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

        return biome;
    }

    void FillInWorldStructure(Chunk* chunk, WorldStructure* structure, int x, int y, int z)
    {
        for (int i = 0; i < structure->p_Structure.size(); i++)
        {
            chunk->SetBlock((BlockType)structure->p_Structure.at(i).block.p_BlockType,
                glm::vec3(x + structure->p_Structure.at(i).x, y + structure->p_Structure.at(i).y,
                    z + structure->p_Structure.at(i).z));    
        }
    }

    void AddWaterBlocks(Chunk* chunk)
    {
        /*
        Generates water in the areas needed inside of the chunk
        */

        ChunkDataTypePtr chunk_data = &chunk->p_ChunkContents;

        for (int i = 0; i < CHUNK_SIZE_X; i++)
        {
            for (int j = 0; j < CHUNK_SIZE_Y; j++)
            {
                for (int k = 0; k < CHUNK_SIZE_Z; k++)
                {
                    if (chunk_data->at(i).at(j).at(k).p_BlockType != BlockType::Air)
                    {
                        continue;
                    }

                    BiomeGenerator.SetNoiseType(FastNoise::Simplex);
                    float real_x = i + chunk->p_Position.x * CHUNK_SIZE_X;
                    float real_z = k + chunk->p_Position.z * CHUNK_SIZE_Z;
                    float biome_noise = BiomeGenerator.GetNoise(real_x, real_z);

                    Biome biome = GetBiome(biome_noise);

                    if (j > water_min && j < water_max)
                    {
                        chunk_data->at(i).at(j).at(k).p_BlockType = BlockType::Water;
                    }
                }
            }
        }
    }

    Biome GetBiome(float chunk_noise)
    {
        // Quantize the noise into various levels and frequency

        const float grass_land = 0.3f;
        const float desert = 0.5f;
        const float ocean = 0.6f;

        if (chunk_noise <= grass_land)
        {
            return Biome::Grassland;
        }

        else if (chunk_noise > desert)
        {
            return Biome::Desert;
        }

        else
        {
            return Biome::Grassland;
        }
    }

    void GenerateChunk(Chunk* chunk, const int WorldSeed, WorldGenerationType gen_type)
    {
        static Random SeedEngine;
        static Random WorldTreeGenerator(WorldSeed);
        static FastNoise WorldGenerator(WorldSeed);
        static FastNoise WorldGeneratorMultiply_1(WorldSeed);

        // Set the chunk state
        chunk->p_ChunkState = ChunkState::Generated;
        WorldGenerator.SetNoiseType(FastNoise::SimplexFractal);

        static TreeStructure WorldStructureTree;
        static CactusStructure WorldStructureCactus; 
        WorldStructure* Structure = nullptr;

        if (gen_type == WorldGenerationType::Generation_Normal)
        {
            float generated_x = 0;
            float generated_y = 0;
            float generated_z = 0;

            // Generates the world using perlin noise to generate a height map

            for (int x = 0; x < CHUNK_SIZE_X; x++)
            {
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    int structure_freq = 0; // The value passed to the seeded random number gen. The higher this number is
                                            // .. the lesser of that structure will be there
                    float real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                    float real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;

                    float height_at = WorldGenerator.GetNoise(real_x, real_z) * WorldGeneratorMultiply_1.GetNoise(real_x * 0.8f, real_z * 0.8f);
                    generated_x = x;
                    generated_z = z;

                    generated_y = (height_at / 2 + 1.0f) * ((float)96);

                    // The biome of the block column
                    Biome biome = SetVerticalBlocks(chunk, generated_x, generated_z, generated_y, real_x, real_z);

                    switch (biome)
                    {
                    case Biome::Grassland:
                        Structure = &WorldStructureTree; 
                        structure_freq = 50;
                        break;

                    case Biome::Desert:
                        Structure = &WorldStructureCactus;
                        structure_freq = 200;
                        break;

                    default:
                        Structure = &WorldStructureTree;
                        structure_freq = 200;
                        break;
                    }

                    if (WorldTreeGenerator.UnsignedInt(75) == 0 &&
                        generated_x + MAX_STRUCTURE_X < CHUNK_SIZE_X &&
                        generated_z + MAX_STRUCTURE_Z < CHUNK_SIZE_Z &&
                        generated_y > water_max + 1 && generated_y < CHUNK_SIZE_Y &&
                        Structure != nullptr)
                    {
                        FillInWorldStructure(chunk, Structure, generated_x, generated_y - 1, generated_z);
                    }
                }
            }

            AddWaterBlocks(chunk);
        }

        else if (gen_type == WorldGenerationType::Generation_Flat || gen_type == WorldGenerationType::Generation_FlatWithoutStructures)
        {
            for (int x = 0; x < CHUNK_SIZE_X; x++)
            {
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    float real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                    float real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;

                    int structure_freq = 0;
                    Biome biome = SetVerticalBlocks(chunk, x, z, 127, real_x, real_z);

                    switch (biome)
                    {
                    case Biome::Grassland:
                        Structure = &WorldStructureTree;
                        structure_freq = 50;
                        break;

                    case Biome::Desert:
                        Structure = &WorldStructureCactus;
                        structure_freq = 200;
                        break;

                    default:
                        Structure = &WorldStructureTree;
                        structure_freq = 200;
                        break;
                    }

                    if (gen_type != WorldGenerationType::Generation_FlatWithoutStructures)
                    {
                        if (WorldTreeGenerator.UnsignedInt(structure_freq) == 0 &&
                            x + MAX_STRUCTURE_X < CHUNK_SIZE_X &&
                            127 + MAX_STRUCTURE_Y < CHUNK_SIZE_Y &&
                            z + MAX_STRUCTURE_Z < CHUNK_SIZE_Z &&
                            Structure != nullptr)
                        {
                            FillInWorldStructure(chunk, Structure, x, 126, z);
                        }
                    }
                }
            }
        }
    }
}