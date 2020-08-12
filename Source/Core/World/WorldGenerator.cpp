#include "WorldGenerator.h"

namespace Minecraft
{
    static FastNoise BiomeGenerator(1234);
    Biome GetBiome(float chunk_noise);

    // Water levels
    constexpr int water_min = 5;
    constexpr int water_max = 85;

    BlockType vein_block = BlockType::Sand;

    BlockType GetUnderwaterBlock()
    {
        int random = rand() % 14;

        switch (random)
        {
        case 0 :
            return BlockType::Sand;
            break;

        case 5 :
            return BlockType::Clay;
            break;
        default:
            return BlockType::Sand;
            break;
        }

        return BlockType::Sand;
    }

    // Sets the vertical blocks based on the biome
    // Also it returns the biome for the block column
    Biome SetVerticalBlocks(Chunk* chunk, int x, int z, int y_level, float real_x, float real_z)
    {
        BiomeGenerator.SetNoiseType(FastNoise::Simplex);

        float column_noise = BiomeGenerator.GetNoise(real_x, real_z);
        Biome biome = GetBiome(column_noise);

        if (y_level >= CHUNK_SIZE_Y)
        {
            y_level = CHUNK_SIZE_Y - 4;
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

        for (int x = 0; x < CHUNK_SIZE_X; x++)
        {
            for (int y = 0; y < CHUNK_SIZE_Y; y++)
            {
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    if (chunk_data->at(x).at(y).at(z).p_BlockType != BlockType::Air)
                    {
                        continue;
                    }

                    if (y > water_min && y < water_max)
                    {
                        BiomeGenerator.SetNoiseType(FastNoise::Simplex);
                        float real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                        float real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;
                        float biome_noise = BiomeGenerator.GetNoise(real_x, real_z);

                        Biome biome = GetBiome(biome_noise);

                        chunk_data->at(x).at(y).at(z).p_BlockType = BlockType::Water;

                        if (x > 0 && y < water_max - 1)
                        {
                            if (chunk_data->at(x - 1).at(y).at(z).IsOpaque())
                            {
                                chunk_data->at(x - 1).at(y).at(z).p_BlockType = GetUnderwaterBlock();
                            }
                        }

                        if (x < CHUNK_SIZE_X - 1 && y < water_max - 1)
                        {
                            if (chunk_data->at(x + 1).at(y).at(z).IsOpaque())
                            {
                                chunk_data->at(x + 1).at(y).at(z).p_BlockType = GetUnderwaterBlock();
                            }
                        }

                        if (z > 0 && y < water_max - 1)
                        {
                            if (chunk_data->at(x).at(y).at(z - 1).IsOpaque())
                            {
                                chunk_data->at(x).at(y).at(z - 1).p_BlockType = GetUnderwaterBlock();
                            }
                        }

                        if (z < CHUNK_SIZE_Z - 1 && y < water_max - 1)
                        {
                            if (chunk_data->at(x).at(y).at(z + 1).IsOpaque())
                            {
                                chunk_data->at(x).at(y).at(z + 1).p_BlockType = GetUnderwaterBlock();
                            }
                        }

                        if (y > 0)
                        {
                            if (chunk_data->at(x).at(y - 1).at(z).IsOpaque())
                            {
                                chunk_data->at(x).at(y - 1).at(z).p_BlockType = GetUnderwaterBlock();
                            }
                        }

                        if (y < water_max)
                        {
                            if (chunk_data->at(x).at(y + 1).at(z).IsOpaque())
                            {
                                chunk_data->at(x).at(y + 1).at(z).p_BlockType = GetUnderwaterBlock();
                            }
                        }
                    }
                }
            }
        }
    }

    Biome GetBiome(float chunk_noise)
    {
        // Quantize the noise into various levels and frequency

        const float grass_land = 0.2f;
        const float desert = 0.2f;
        const float ocean = 0.6f;

        if (chunk_noise <= grass_land)
        {
            return Biome::Grassland;
        }

        else if (chunk_noise > desert)
        {
            return Biome::Desert;
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

                    float height_at = WorldGenerator.GetNoise(real_x, real_z) +
                        (0.5 * WorldGenerator.GetNoise(real_x, real_z)) *
                        WorldGeneratorMultiply_1.GetNoise(real_x * 0.4f, real_z * 0.4f);
                    generated_x = x;
                    generated_z = z;

                    generated_y = (height_at / 2 + 1.0f) * ((float)95);

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
                        structure_freq = 75;
                        break;

                    default:
                        Structure = &WorldStructureTree;
                        structure_freq = 50;
                        break;
                    }

                    if (WorldTreeGenerator.UnsignedInt(structure_freq) == 0 &&
                        generated_x + MAX_STRUCTURE_X < CHUNK_SIZE_X &&
                        generated_z + MAX_STRUCTURE_Z < CHUNK_SIZE_Z &&
                        generated_y > water_max + 1 && generated_y < CHUNK_SIZE_Y &&
                        Structure != nullptr)
                    {
                        FillInWorldStructure(chunk, Structure, generated_x, generated_y - 2, generated_z);
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