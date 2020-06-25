#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "ChunkMesh.h"

namespace Minecraft
{
	class Chunk
	{
	public : 

		Chunk(const glm::vec2& chunk_pos)
		{
			m_ChunkPosition = chunk_pos;

			// Initialize all the blocks in the chunk to be air blocks

			for (int i = 0; i < ChunkSizeX; i++)
			{
				for (int j = 0; j < ChunkSizeY; j++)
				{
					for (int k = 0; k < ChunkSizeZ; k++)
					{
						m_ChunkContents[i][j][k].p_BlockType = BlockType::Air;
					}
				}
			}
		}

		void AddBlock(BlockType type, glm::vec3 position)
		{
			Block b;
			b.p_Position = position;
			b.p_Chunk = this;
			b.p_BlockType = type;

			m_ChunkContents[position.x][position.y][position.z] = b;
		}

		void Construct()
		{
			m_ChunkMesh.ConstructMesh(&m_ChunkContents);
		}

		ChunkMesh* GetChunkMesh()
		{
			return &m_ChunkMesh;
		}

	private :
		// each chunk will be a 16x16x16 block space. A 3D array

		std::array<std::array<std::array<Block, ChunkSizeX>, ChunkSizeY>, ChunkSizeZ> m_ChunkContents;
		glm::vec2 m_ChunkPosition;
		ChunkMesh m_ChunkMesh;
	};
}