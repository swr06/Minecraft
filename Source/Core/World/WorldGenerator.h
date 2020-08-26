#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

// Include fast noise
#include "../Noise/FastNoise.h"

#include "World.h"
#include "WorldGeneratorType.h"
#include "Structures/WorldStructures.h"
#include "../Chunk.h"
#include "../Utils/Random.h"
#include "../Utils/Defs.h"

namespace Minecraft
{
	Block* GetWorldBlock(const glm::vec3& block_pos);

	enum Biome
	{
		Grassland = 9,
		Plains,
		Ocean,
		Desert,
		Jungle
	};

	void GenerateChunk(Chunk* chunk, const int WorldSeed, WorldGenerationType gen_type);
	void GenerateChunkFlora(Chunk* chunk, const int WorldSeed, WorldGenerationType gen_type);
}