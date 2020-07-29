#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ChunkMesh.h"

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

			for (int i = 0; i < CHUNK_SIZE_X; i++)
			{
				for (int j = 0; j < CHUNK_SIZE_Y; j++)
				{
					for (int k = 0; k < CHUNK_SIZE_Z; k++)
					{
						p_ChunkContents.at(i).at(j).at(k).p_BlockType = BlockType::Air;
					}
				}
			}

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

		void Construct()
		{
			m_ChunkMesh.ConstructMesh(&p_ChunkContents, p_Position);
		}

		ChunkMesh* GetChunkMesh()
		{
			return &m_ChunkMesh;
		}

		const glm::vec3 p_Position;
		ChunkMeshState p_MeshState = ChunkMeshState::Unbuilt;
		ChunkState p_ChunkState = ChunkState::Ungenerated;
		std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> p_ChunkContents;


	private :
		// each chunk will be a 16x16x16 block space. A 3D array


		ChunkMesh m_ChunkMesh;
	};
}