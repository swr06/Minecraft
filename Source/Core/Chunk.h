#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Maths/Frustum.h"
#include "ChunkMesh.h"
#include "Lighting/Lighting.h"
#include "World/Biome.h"

namespace Minecraft
{
	enum class ChunkMeshState : std::uint8_t
	{
		Built = 0,
		Unbuilt,
		Edited,
		error
	};

	enum class ChunkLightMapState
	{
		ModifiedLightMap = 10,
		UnmodifiedLightMap
	};

	enum class ChunkState
	{
		Ungenerated = 5,
		Generated,
		Changed,
		undefined
	};

	class Chunk
	{
	public : 

		Chunk(const glm::vec3 chunk_position);
		~Chunk();

		void SetBlock(BlockType type, const glm::vec3& position);

		int GetTorchLightAt(int x, int y, int z);
		void SetTorchLightAt(int x, int y, int z, int light_val);

		void Construct();
		ChunkMesh* GetChunkMesh();

		Block* GetBlock(int x, int y, int z);

		const glm::vec3 p_Position;
		ChunkMeshState p_MeshState;
		ChunkState p_ChunkState = ChunkState::Ungenerated;
		std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> p_ChunkContents;
		std::array<std::array<std::array<uint8_t, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> p_ChunkLightInformation;
		ChunkLightMapState p_LightMapState;
		FrustumAABB p_ChunkFrustumAABB;

		// Total : 512 bytes for the height and biome maps
		std::array<std::array<uint8_t, CHUNK_SIZE_X>, CHUNK_SIZE_Z> p_HeightMap;
		std::array<std::array<Biome, CHUNK_SIZE_X>, CHUNK_SIZE_Z> p_BiomeMap;

	private :
		ChunkMesh m_ChunkMesh;
	};
}