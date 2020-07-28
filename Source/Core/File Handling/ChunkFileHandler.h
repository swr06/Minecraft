#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>

#include "../Chunk.h"
#include "../Utils/Logger.h"
#include "../Utils/Defs.h"
#include "../Block.h"

namespace Minecraft
{
	namespace ChunkFileHandler
	{
		// Reads or writes a chunk to a file with a unique filename
		// Uses C style file handling because it's slightly faster than the C++ counterpart

		bool WriteChunk(Chunk* chunk, const std::string& dir);
		bool ReadChunk(Chunk* chunk, const std::string& dir);
	}
}