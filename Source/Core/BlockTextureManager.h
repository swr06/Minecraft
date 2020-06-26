#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <array>
#include "Block.h"
#include "TextureAtlas.h"

namespace Minecraft
{
	enum BlockFaceType
	{
		top = 0,
		bottom,
		left,
		right,
		front,
		backward
	};

	const std::array<GLfloat, 8>& GetBlockTexture(BlockType block_type, BlockFaceType face_type);
}