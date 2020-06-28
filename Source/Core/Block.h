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
		Dirt = 0,
		Stone,
		Leaf,
		Wood,
		Cactus,
		Water,
		Air
	};

	struct Block
	{
		BlockType p_BlockType;
		glm::vec3 p_Position;
		Chunk* p_Chunk;
	};
}