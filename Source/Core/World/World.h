#pragma once

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "../Chunk.h"
#include "../Block.h"
#include "../Utils/Defs.h"
#include "../Utils/Logger.h"
#include "../Utils/Timer.h"
#include "../Player/Player.h"
#include "../Renderer/Renderer.h"
#include "../Application/Events.h"
#include "WorldGenerator.h"

namespace Minecraft
{
	class World
	{
	public : 

		World();
		~World();

		void OnUpdate(GLFWwindow* window);
		void RenderWorld();
		void OnEvent(EventSystem::Event e);

		Player* p_Player;

	private:
		Renderer m_Renderer;
		std::array<std::array<Chunk, 16>, 16> m_WorldChunks;
		std::map<glm::vec3, Chunk> m_Chunks;
		int m_ChunkCount;
	};
}