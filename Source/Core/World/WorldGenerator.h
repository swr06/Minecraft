#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include fast noise
#include "../Noise/FastNoise.h"

#include "World.h"
#include "Structures/WorldStructures.h"
#include "../Chunk.h"
#include "../Utils/Random.h"
#include "../Utils/Defs.h"

namespace Minecraft
{
	void GenerateChunk(Chunk* chunk);

}