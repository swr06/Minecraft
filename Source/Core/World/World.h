#pragma once

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <vector>

#include "../Chunk.h"
#include "../Block.h"
#include "../Utils/Defs.h"
#include "../Player/Player.h"
#include "../Renderer/Renderer.h"

namespace Minecraft
{
	class World
	{
	public : 

		World();
		~World();

		void OnUpdate();
		void RenderWorld();
		void OnEvent();

		Player* p_Player;

	private:
		Renderer m_Renderer;
		std::map<glm::vec3, Chunk> m_Chunks;
		int m_ChunkCount;
	};
}