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
	class Chunk
	{
	public : 

		Chunk()
		{
			// Initialize all the blocks in the chunk to be air blocks

			m_ChunkContents = new std::array<std::array<std::array<Block, ChunkSizeX>, ChunkSizeY>, ChunkSizeZ>;

			for (int i = 0; i < ChunkSizeX; i++)
			{
				for (int j = 0; j < ChunkSizeY; j++)
				{
					for (int k = 0; k < ChunkSizeZ; k++)
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
			b.p_Position = position;
			b.p_BlockType = type;

			m_ChunkContents->at(position.x).at(position.y).at(position.z) = b;
		}

		void SetBlock(Block* block)
		{
			m_ChunkContents->at(block->p_Position.x).at(block->p_Position.y).at(block->p_Position.z) = *block;
		}

		void Construct()
		{
			m_ChunkMesh.ConstructMesh(m_ChunkContents, p_Position);
		}

		ChunkMesh* GetChunkMesh()
		{
			return &m_ChunkMesh;
		}

		glm::vec3 p_Position;
		std::array<std::array<std::array<Block, ChunkSizeX>, ChunkSizeY>, ChunkSizeZ>* m_ChunkContents;

	private :
		// each chunk will be a 16x16x16 block space. A 3D array

		ChunkMesh m_ChunkMesh;
	};
}