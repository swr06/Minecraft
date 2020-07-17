#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Defs.h"

namespace Minecraft
{
	class Chunk;

	enum BlockType
	{
		Grass = 0,
		Dirt,
		Stone,
		Leaf,
		Wood,
		Cactus,
		Sand,
		Water,
		Air
	};

	struct Block
	{
		BlockType p_BlockType;
	};
}