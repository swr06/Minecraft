#include "WorldGenerator.h"

namespace Minecraft
{
    static FastNoise BiomeGenerator(1234);
    Biome GetBiome(float chunk_noise);

    // Water levels
    constexpr int water_min = 2;
    constexpr int water_max = 90;

    BlockType vein_block = BlockType::Sand;

    BlockType GetUnderwaterBlock(Random& random_gen)
    {
        switch (random_gen.UnsignedInt(6))
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
    void SetVerticalBlocks(Chunk* chunk, int x, int z, int y_level, float real_x, float real_z)
    {
        BiomeGenerator.SetNoiseType(FastNoise::Simplex);

        float column_noise = BiomeGenerator.GetNoise(real_x, real_z);
        Biome biome = GetBiome(column_noise);

        for (int i = 0; i < y_level; i++)
        {
            if (i > 1)
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

            else
            {
                chunk->SetBlock(BlockType::Bedrock, glm::vec3(x, i, z));
            }
        }

        return;
    }

    void AddWaterBlocks(Chunk* chunk)
    {
        /*
        Generates water in the areas needed inside of the chunk
        */

        Random water_gen(chunk->p_Position.x + chunk->p_Position.y);
        ChunkDataTypePtr chunk_data = &chunk->p_ChunkContents;
        int cx = chunk->p_Position.x;
        int cz = chunk->p_Position.z;

        for (int x = 0; x < CHUNK_SIZE_X; x++)
        {
            for (int y = 0; y < CHUNK_SIZE_Y; y++)
            {
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    Block* block = &chunk_data->at(x).at(y).at(z);

                    if (block->p_BlockType == BlockType::Air)
                    {
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
                                    chunk_data->at(x - 1).at(y).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                                }
                            }

                            if (x < CHUNK_SIZE_X - 1 && y < water_max - 1)
                            {
                                if (chunk_data->at(x + 1).at(y).at(z).IsOpaque())
                                {
                                    chunk_data->at(x + 1).at(y).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                                }
                            }

                            if (z > 0 && y < water_max - 1)
                            {
                                if (chunk_data->at(x).at(y).at(z - 1).IsOpaque())
                                {
                                    chunk_data->at(x).at(y).at(z - 1).p_BlockType = GetUnderwaterBlock(water_gen);
                                }
                            }

                            if (z < CHUNK_SIZE_Z - 1 && y < water_max - 1)
                            {
                                if (chunk_data->at(x).at(y).at(z + 1).IsOpaque())
                                {
                                    chunk_data->at(x).at(y).at(z + 1).p_BlockType = GetUnderwaterBlock(water_gen);
                                }
                            }

                            if (y > 0)
                            {
                                if (chunk_data->at(x).at(y - 1).at(z).IsOpaque())
                                {
                                    chunk_data->at(x).at(y - 1).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                                }
                            }

                            if (y < water_max)
                            {
                                if (chunk_data->at(x).at(y + 1).at(z).IsOpaque())
                                {
                                    chunk_data->at(x).at(y + 1).at(z).p_BlockType = GetUnderwaterBlock(water_gen);
                                }
                            }
                        }
                    }

                    else if (block->p_BlockType != BlockType::Air && y > water_max - 2&& y < water_max + 4)
                    {
                        chunk_data->at(x).at(y).at(z) = { BlockType::Sand };
                    }
                }
            }
        }
    }

    BlockType GenerateFlower()
    {
        static Random random;

        switch (random.UnsignedInt(6))
        {
        case 0:
            return BlockType::Flower_allium;
            break;
        case 1:
            return BlockType::Flower_orchid;
            break;
        case 2:
            return BlockType::Flower_tulip_red;
            break;

        case 3:
            return BlockType::Flower_tulip_pink;
            break;
        case 4:
            return BlockType::Flower_rose;
            break;

        case 5:
            return BlockType::Flower_dandelion;
            break;

        default :
            return BlockType::Flower_tulip_red;
            break;
        }
        return BlockType::Flower_tulip_red;
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

    void GenerateCaves(Chunk* chunk, const int seed)
    {
        constexpr int cave_level = 55;

        static FastNoise noise_1(seed);
        static FastNoise noise_2(seed);

        noise_1.SetNoiseType(FastNoise::Simplex);
        noise_2.SetNoiseType(FastNoise::Simplex);

        for (int x = 0; x < CHUNK_SIZE_X; x++)
        {
            for (int y = 0; y < CHUNK_SIZE_Y; y++)
            {
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    if (y < 2)
                    {
                        chunk->p_ChunkContents.at(x).at(y).at(z) = { BlockType::Bedrock };
                    }

                    else if (y < cave_level)
                    {
                        float real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                        float real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;

                        float n1 = noise_1.GetNoise(real_x, y, real_z);
                        n1 = n1 * n1;

                        float n2 = noise_2.GetNoise(real_x, y, real_z);
                        n2 = n2 * n2;

                        float res = n1 + n2;

                        bool cave = res < 0.02 ? true : false;

                        if (cave)
                        {
                            chunk->p_ChunkContents.at(x).at(y).at(z) = { BlockType::Air };
                        }
                    }
                }
            }
        }
    }

    float _rounded(const glm::vec2& coord)
    {
        auto bump = [](float t) { return glm::max(0.0f, 1.0f - std::pow(t, 6.0f)); };
        float b = bump(coord.x) * bump(coord.y);
        return b * 0.9f;
    }

    TreeStructure WorldStructureTree;
    CactusStructure WorldStructureCactus;
    Random WorldTreeGenerator(1234);
    Random WorldModelGenerator(1234);

    bool FillInWorldStructure(WorldStructure* structure, int x, int y, int z)
    {
        if (y <= 0 || y >= CHUNK_SIZE_Y)
        {
            return false;
        }

        for (int i = 0; i < structure->p_Structure.size(); i++)
        {
            Block* block = GetWorldBlock(glm::vec3(x + structure->p_Structure.at(i).x, y + structure->p_Structure.at(i).y, z + structure->p_Structure.at(i).z));
            *block = structure->p_Structure.at(i).block;
        }

        return true;
    }

    void GenerateChunkFlora(Chunk* chunk, const int WorldSeed, WorldGenerationType gen_type)
    {
        if (gen_type == WorldGenerationType::Generation_FlatWithoutStructures)
        {
            return;
        }

        for (int x = 0; x < CHUNK_SIZE_X; x++)
        {
            for (int z = 0; z < CHUNK_SIZE_Z; z++)
            {
                int real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                int real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;
                int structure_freq = 0; // The higher the less likely it is to spawn
                bool added_structure = false;

                float column_noise = BiomeGenerator.GetNoise(real_x, real_z);
                Biome biome = GetBiome(column_noise);

                if (chunk->p_HeightMap[x][z] + MAX_STRUCTURE_Y < CHUNK_SIZE_Y)
                {
                    int structure_x = real_x;
                    int structure_y = chunk->p_HeightMap[x][z];
                    int structure_z = real_z;

                    WorldStructure* structure = nullptr;

                    if (biome == Biome::Grassland)
                    {
                        structure_x = real_x - 2;
                        structure_z = real_z - 2;
                        structure_freq = 200;
                        structure = &WorldStructureTree;
                    }

                    else if (biome == Biome::Desert)
                    {
                        structure_freq = 300;
                        structure = &WorldStructureCactus;
                    }

                    if (structure && WorldTreeGenerator.UnsignedInt(structure_freq) == 0 && chunk->p_HeightMap[x][z] > water_max + 6)
                    {
                        added_structure = true;
                        FillInWorldStructure(structure, structure_x, structure_y, structure_z);
                    }
                }

                if (chunk->p_HeightMap[x][z] + 1 < CHUNK_SIZE_Y && chunk->p_HeightMap[x][z] + 1 > water_max + 6 && !added_structure)
                {
                    int y = chunk->p_HeightMap[x][z];
                    BlockType model_type = BlockType::Air;

                    if (biome == Biome::Grassland)
                    {
                        int num = WorldModelGenerator.UnsignedInt(150);

                        if (num > 135)
                        {
                            model_type = BlockType::Model_Grass;
                        }

                        else if (num == 40 || num == 30)
                        {
                            model_type = GenerateFlower();
                        }
                    }

                    else if (biome == Biome::Desert)
                    {
                        int num = WorldModelGenerator.UnsignedInt(280);

                        if (num == 40 || num == 30)
                        {
                            model_type = BlockType::Model_Deadbush;
                        }
                    }

                    chunk->p_ChunkContents.at(x).at(y).at(z).p_BlockType = model_type;
                }
            }
        }
    }

    void GenerateChunk(Chunk* chunk, const int WorldSeed, WorldGenerationType gen_type)
    {
        static FastNoise WorldGenerator(WorldSeed);
        static FastNoise WorldGeneratorMultiply_1(WorldSeed);

        // Set the chunk state
        WorldGenerator.SetNoiseType(FastNoise::Simplex);
        BiomeGenerator.SetNoiseType(FastNoise::Simplex);

        if (gen_type == WorldGenerationType::Generation_Normal || gen_type == WorldGenerationType::Generation_Normal_withoutcaves)
        {
            int generated_x = 0;
            int generated_y = 0;
            int generated_z = 0;

            // Generates the world using perlin noise to generate a height map

            for (int x = 0; x < CHUNK_SIZE_X; x++)
            {
                for (int z = 0; z < CHUNK_SIZE_Z; z++)
                {
                    int real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                    int real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;

                    float height_at = (1.0f * WorldGenerator.GetNoise(1.0f * real_x, 1.0f * real_z)) +
                        (0.5f * WorldGenerator.GetNoise(2.0f * real_x, 2.0f * real_z)) +
                        (0.25f * WorldGenerator.GetNoise(4.0f * real_x, 4.0f * real_z));

                    constexpr float multiply_factor = 0.30f;
                    height_at *= WorldGenerator.GetNoise(multiply_factor * real_x, (multiply_factor)* real_z);

                    generated_x = x;
                    generated_z = z;

                    float multiplication_factor = 200.0f;

                    generated_y = ((height_at + 1.0f) / 2) * multiplication_factor;
                    chunk->p_HeightMap[x][z] = static_cast<uint8_t>(generated_y);

                    if (generated_y >= CHUNK_SIZE_Y)
                    {
                        generated_y = CHUNK_SIZE_Y - 2;
                    }

                    SetVerticalBlocks(chunk, generated_x, generated_z, generated_y, real_x, real_z);
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
                    const uint8_t height = 127;

                    float real_x = x + chunk->p_Position.x * CHUNK_SIZE_X;
                    float real_z = z + chunk->p_Position.z * CHUNK_SIZE_Z;
                    chunk->p_HeightMap[x][z] = height;

                    SetVerticalBlocks(chunk, x, z, height, real_x, real_z);
                }
            }
        }
    }
}