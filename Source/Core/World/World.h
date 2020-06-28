#pragma once

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		void RenderChunkFromMap(int cx, int cz);

		Renderer m_Renderer;
		//std::array<std::array<Chunk, 16>, 16> m_WorldChunks;
		std::map<int, std::map<int, Chunk>> m_WorldChunks;
		int m_ChunkCount;
	};
}