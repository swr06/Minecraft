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

	class Chunk
	{
	public : 

		Chunk(const glm::vec3 chunk_position) : p_Position(chunk_position)
		{
			// Initialize all the blocks in the chunk to be air blocks

			memset(&p_ChunkContents, BlockType::Air, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
			memset(&p_ChunkLightInformation, 0, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
		}

		~Chunk()
		{
			
		}

		void SetBlock(BlockType type, const glm::vec3& position)
		{
			Block b;
			b.p_BlockType = type;

			p_ChunkContents.at(position.x).at(position.y).at(position.z) = b;
		}

		inline int GetSunlightAt(int x, int y, int z)
		{
			 // Shift four bits so you only get the sunlight info
			 // Then & that which 15 (0xF)
			return (p_ChunkLightInformation[x][y][z] >> 4) & 0xF;
		}

		inline int GetTorchLightAt(int x, int y, int z)
		{
			return (p_ChunkLightInformation[x][y][z]) & 0xF;
		}

		inline void SetSunlightAt(int x, int y, int z, int light_val)
		{
			p_ChunkLightInformation[x][y][z] = (p_ChunkLightInformation[x][y][z] & 0xF) | (light_val << 4);
		}

		inline void SetTorchLightAt(int x, int y, int z, int light_val)
		{
			p_ChunkLightInformation[x][y][z] = (p_ChunkLightInformation[x][y][z] & 0xF0) | light_val;
		}

		void Construct()
		{
			m_ChunkMesh.ConstructMesh(&p_ChunkContents, p_Position);
		}

		ChunkMesh* GetChunkMesh()
		{
			return &m_ChunkMesh;
		}

		Block* GetBlock(int x, int y, int z)
		{
			return &p_ChunkContents[x][y][z];
		}

		const glm::vec3 p_Position;
		ChunkMeshState p_MeshState = ChunkMeshState::Unbuilt;
		ChunkState p_ChunkState = ChunkState::Ungenerated;
		std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> p_ChunkContents;

		// total : 8 bits
		// first 4 bits for torch light
		// lasr 4 bits for sunling
		std::array<std::array<std::array<unsigned char, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> p_ChunkLightInformation;

	private :
		// each chunk will be a 16x16x16 block space. A 3D array


		ChunkMesh m_ChunkMesh;
	};
}