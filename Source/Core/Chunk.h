#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ChunkMesh.h"
#include "Lighting/Lighting.h"

namespace Minecraft
{
	enum class ChunkMeshState : std::uint8_t
	{
		Built = 0,
		Unbuilt,
		Edited,
		error
	};

	enum class ChunkState
	{
		Ungenerated = 5,
		Generated,
		Changed,
		undefined
	};

	struct i8Vec2
	{
		std::int8_t x;
		std::int8_t y;
	};

	class Chunk
	{
	public : 

		Chunk(const glm::vec3 chunk_position);
		~Chunk();

		void SetBlock(BlockType type, const glm::vec3& position);

		int GetSunlightAt(int x, int y, int z);
		int GetTorchLightAt(int x, int y, int z);

		void SetSunlightAt(int x, int y, int z, int light_val);
		void SetTorchLightAt(int x, int y, int z, int light_val);

		void Construct();
		ChunkMesh* GetChunkMesh();

		Block* GetBlock(int x, int y, int z);

		const glm::vec3 p_Position;
		ChunkMeshState p_MeshState;
		ChunkState p_ChunkState;
		std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> p_ChunkContents;

		// total : 8 bits
		// first 4 bits for torch light
		// lasr 4 bits for sunling
		std::array<std::array<std::array<i8Vec2, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> p_ChunkLightInformation;

	private :
		// each chunk will be a 16x16x16 block space. A 3D array

		ChunkMesh m_ChunkMesh;
	};
}