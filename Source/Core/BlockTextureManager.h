#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <array>
#include "Block.h"
#include "TextureAtlas.h"
#include "Utils/Enums.h"

namespace Minecraft
{
	const std::array<GLfloat, 8>& GetBlockTexture(BlockType block_type, BlockFaceType face_type);
}