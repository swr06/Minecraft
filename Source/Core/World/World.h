#pragma once

// Use legacy "unsafe" functions
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <vector>
#include <thread>

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
#include "../Utils/Ray.h"
#include "../Physics/AABB.h"
#include "../Clouds/Cloud.h"
#include "../CubeRenderer.h"
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
		std::pair<Block*, Chunk*> GetBlockFromPosition(const glm::vec3& pos);
		void SetBlockFromPosition(BlockType type, const glm::vec3& pos);
		std::pair<Block*, Chunk*> GetBlock(const glm::vec3& block_loc);
		BlockType GetBlockTypeFromPosition(const glm::vec3& pos);
		glm::vec3 ConvertPositionToWorldBlockPosition(const glm::vec3& pos);
		bool ChunkExistsInMap(int cx, int cz);
		Chunk* RetrieveChunkFromMap(int cx, int cz);

		Player* p_Player;

		const std::map<std::pair<int, int>, Chunk>& GetWorldData()
		{
			return m_WorldChunks;
		}

		Chunk* EmplaceChunkInMap(int cx, int cz);
	private:
		void RenderChunkFromMap(int cx, int cz);
		void UnloadFarChunks();
		void RayCast(bool place);
		void DoCollisionTests();
		void UpdateSurroundingChunks(int cx, int cz);

	
		bool ChunkInViewFrustum(Chunk* chunk);

		uint32_t m_ChunkCount;
		std::pair<float, float> m_CrosshairPosition;

		// Renderers
		Renderer m_Renderer;
		Renderer2D m_Renderer2D;
		CubeRenderer m_CubeRenderer;
		
		std::map<std::pair<int, int>, Chunk> m_WorldChunks;
		Skybox m_Skybox;
		glm::vec3 m_StartRay;
		glm::vec3 m_EndRay;

		OrthographicCamera m_Camera2D;
		CloudManager m_CloudManager;
		GLClasses::Texture m_CrosshairTexture;
		std::uint8_t m_CurrentHeldBlock = 0;
		long long m_CurrentFrame;
	};
}