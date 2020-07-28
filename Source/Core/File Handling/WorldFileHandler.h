#pragma once

#include "ChunkFileHandler.h"
#include "../World/World.h"

#include <iostream>
#include <string>
#include <filesystem>

namespace Minecraft
{
	namespace WorldFileHandler
	{
		bool SaveWorld(const std::string& world_name, World* world);
		World* LoadWorld(const std::string& world_name);
	}
}