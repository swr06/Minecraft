#pragma once

// Use legacy "unsafe" functions
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <vector>
#include <thread>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "../Utils/Raycast.h"

#include "Skybox.h"
#include "../Chunk.h"
#include "../Block.h"
#include "../Utils/Defs.h"
#include "../Utils/Logger.h"
#include "../Utils/Timer.h"
#include "../Player/Player.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Renderer2D.h"
#include "../Application/Events.h"
#include "../ChunkFileHandler.h"
#include "../Utils/Ray.h"
#include "WorldGenerator.h"

// TODO : Include ray cast class

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

		// Gets a world block from the respective chunk. Returns nullptr if invalid
		std::pair<Block*, Chunk*> GetWorldBlockFromPosition(const glm::vec3& block_loc);
		void SetWorldBlockFromPosition(BlockType type, const glm::vec3& block_loc);
		BlockType GetWorldBlockTypeFromPosition(const glm::vec3& block_loc);

		Player* p_Player;


	private:
		void RenderChunkFromMap(int cx, int cz);
		void UnloadFarChunks();

		Chunk* GetChunkFromMap(int cx, int cz);
		bool ChunkInViewFrustum(Chunk* chunk);

		uint32_t m_ChunkCount;

		// Renderers
		Renderer m_Renderer;
		Renderer2D m_Renderer2D;
		
		std::map<std::pair<int, int>, Chunk> m_WorldChunks;
		Skybox m_Skybox;
		glm::vec3 m_StartRay;
		glm::vec3 m_EndRay;

		OrthographicCamera m_Camera2D;
		GLClasses::Texture m_CrosshairTexture;
	};
}