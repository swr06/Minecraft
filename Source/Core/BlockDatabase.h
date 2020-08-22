#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <array>
#include <unordered_map>
#include "Block.h"
#include "TextureAtlas.h"
#include "Utils/Enums.h"

namespace Minecraft
{
	namespace BlockDatabase
	{
		const std::array<uint16_t, 8>& GetBlockTexture(BlockType block_type, BlockFaceType face_type);
		const std::string& GetBlockName(BlockType block_type);
	}
}