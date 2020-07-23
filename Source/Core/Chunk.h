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
	enum ChunkMeshState : std::uint8_t
	{
		Built = 0,
		Unbuilt,
		Edited,
		error
	};

	class Chunk
	{
	public : 

		Chunk(const glm::vec3 chunk_position) : p_Position(chunk_position)
		{
			// Initialize all the blocks in the chunk to be air blocks

			m_ChunkContents = new std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>;

			for (int i = 0; i < CHUNK_SIZE_X; i++)
			{
				for (int j = 0; j < CHUNK_SIZE_Y; j++)
				{
					for (int k = 0; k < CHUNK_SIZE_Z; k++)
					{
						m_ChunkContents->at(i).at(j).at(k).p_BlockType = BlockType::Air;
					}
				}
			}

		}

		~Chunk()
		{
			delete m_ChunkContents;
		}

		void SetBlock(BlockType type, const glm::vec3& position)
		{
			Block b;
			b.p_BlockType = type;

			m_ChunkContents->at(position.x).at(position.y).at(position.z) = b;
		}

		void Construct()
		{
			m_ChunkMesh.ConstructMesh(m_ChunkContents, p_Position);
		}

		ChunkMesh* GetChunkMesh()
		{
			return &m_ChunkMesh;
		}

		const glm::vec3 p_Position;
		std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>* m_ChunkContents;
		ChunkMeshState p_MeshState = ChunkMeshState::Unbuilt;

	private :
		// each chunk will be a 16x16x16 block space. A 3D array

		ChunkMesh m_ChunkMesh;
	};
}