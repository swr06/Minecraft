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
#include "../Clouds/Cloud.h"
#include "../CubeRenderer.h"
#include "WorldGenerator.h"

namespace Minecraft
{
	enum WorldGenerationType 
	{
		Generation_Normal = 0,
		Generation_Hilly,
		Generation_Flat,
		Generation_FlatWithoutStructures
	};

	enum CurrentSunCycle
	{
		Sun_Setting,
		Sun_Rising
	};

	class World
	{
	public:

		World(int seed, const glm::vec2& window_size, const std::string& world_name, WorldGenerationType world_gen_type);
		~World();

		void OnUpdate(GLFWwindow* window);
		void RenderWorld();
		void OnEvent(EventSystem::Event e);
		const std::string& GetName() noexcept { return m_WorldName; }

		// Gets a world block from the respective chunk. Returns nullptr if invalid
		std::pair<Block*, Chunk*> GetBlockFromPosition(const glm::vec3& pos) noexcept;
		std::pair<Block*, Chunk*> GetBlock(const glm::vec3& block_loc) noexcept;
		BlockType GetBlockTypeFromPosition(const glm::vec3& pos) noexcept;
		Chunk* RetrieveChunkFromMap(int cx, int cz) noexcept;
		WorldGenerationType GetWorldGenerationType() { return m_WorldGenType; }

		bool ChunkExistsInMap(int cx, int cz);
		void SetBlockFromPosition(BlockType type, const glm::vec3& pos);

		Player* p_Player;

		const std::map<std::pair<int, int>, Chunk>& GetWorldData()
		{
			return m_WorldChunks;
		}

		Chunk* EmplaceChunkInMap(int cx, int cz);
		int GetSeed();

	private:

		void UnloadFarChunks();
		void RayCast(bool place);
		void DoCollisionTests();
		void UpdateSurroundingChunks(int cx, int cz);
		void UpdateLights();
		void TickSun();
		bool TestRayPlayerCollision(const glm::vec3& ray_block);

		uint32_t m_ChunkCount;
		std::pair<float, float> m_CrosshairPosition;

		// Renderers
		Renderer m_Renderer;
		Renderer2D m_Renderer2D;

		std::map<std::pair<int, int>, Chunk> m_WorldChunks;
		Skybox m_Skybox;
		glm::vec3 m_StartRay;
		glm::vec3 m_EndRay;

		OrthographicCamera m_Camera2D;
		//CloudManager m_CloudManager;
		GLClasses::Texture m_CrosshairTexture;
		long long m_CurrentFrame;
		const int m_WorldSeed;

		// Lighting
		std::queue<LightNode> m_LightBFSQueue;
		std::queue<LightRemovalNode> m_LightRemovalBFSQueue;

		// Day and night cycle
		glm::vec4 m_SunPosition;
		CurrentSunCycle m_SunCycle;

		ViewFrustum m_ViewFrustum;

		const std::string m_WorldName;
		WorldGenerationType m_WorldGenType;
	};
}