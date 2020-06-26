#include "ChunkMesh.h"

namespace Minecraft
{
	ChunkMesh::ChunkMesh() : p_VBO(GL_ARRAY_BUFFER)
	{
		p_VAO.Bind();
		p_VBO.Bind();
		p_VBO.BufferData((ChunkSizeX * ChunkSizeY * ChunkSizeZ * sizeof(Vertex) * 6) + 16, nullptr, GL_DYNAMIC_DRAW);
		p_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		p_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		p_VBO.VertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
		p_VAO.Unbind();

		// Set the values of the 2D planes

		m_ForwardFace[0] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
		m_ForwardFace[1] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
		m_ForwardFace[2] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_ForwardFace[3] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);

		m_BackFace[0] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
		m_BackFace[1] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
		m_BackFace[2] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
		m_BackFace[3] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);

		m_TopFace[0] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
		m_TopFace[1] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
		m_TopFace[2] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_TopFace[3] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);

		m_BottomFace[0] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
		m_BottomFace[1] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
		m_BottomFace[2] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
		m_BottomFace[3] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
		
		m_LeftFace[0] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);
		m_LeftFace[1] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
		m_LeftFace[2] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
		m_LeftFace[3] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);

		m_RightFace[0] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_RightFace[1] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
		m_RightFace[2] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
		m_RightFace[3] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
	}

	void ChunkMesh::ConstructMesh(std::array<std::array<std::array<Block, ChunkSizeX>, ChunkSizeY>, ChunkSizeZ>* Chunk, const glm::vec3& chunk_pos)
	{
		glm::vec3 world_position;
		p_Vertices.erase(p_Vertices.begin(), p_Vertices.end());

		for (int x = 0; x < ChunkSizeX; x++)
		{
			for (int y = 0; y < ChunkSizeY; y++)
			{
				for (int z = 0; z < ChunkSizeZ; z++)
				{
					if (Chunk->at(x).at(y).at(z).p_BlockType != BlockType::Air)
					{
						world_position = Chunk->at(x).at(y).at(z).p_Position;
						
						world_position.x = chunk_pos.x * ChunkSizeX + Chunk->at(x).at(y).at(z).p_Position.x;
						world_position.y = chunk_pos.y * ChunkSizeY + Chunk->at(x).at(y).at(z).p_Position.y;
						world_position.z = chunk_pos.z * ChunkSizeZ + Chunk->at(x).at(y).at(z).p_Position.z;

						if (z <= 0)
						{
							AddFace(BlockFaceType::backward, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
						}

						else if (z >= ChunkSizeZ - 1)
						{
							AddFace(BlockFaceType::front, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
						}

						else
						{
							//If the forward block is an air block, add the forward face to the mesh
							if (Chunk->at(x).at(y).at(z + 1).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::front, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
							}

							// If the back (-forward) block is an air block, add the back face to the mesh
							if (Chunk->at(x).at(y).at(z - 1).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::backward, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
							}
						}

						if (x <= 0)
						{
							AddFace(BlockFaceType::left, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
						}

						else if (x >= ChunkSizeX - 1)
						{
							AddFace(BlockFaceType::right, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
						}

						else
						{
							// If the next block is an air block, add the right face to the mesh
							if (Chunk->at(x + 1).at(y).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::right, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
							}

							// If the previous block is an air block, add the left face to the mesh
							if (Chunk->at(x - 1).at(y).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::left, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
							}
						}

						if (y <= 0)
						{
							AddFace(BlockFaceType::bottom, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
						}

						else if (y >= ChunkSizeY - 1)
						{
							AddFace(BlockFaceType::top, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
						}

						else
						{
							// If the top block is an air block, add the top face to the mesh
							if (Chunk->at(x).at(y - 1).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::bottom, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
							}

							// If the bottom block is an air block, add the top face to the mesh
							if (Chunk->at(x).at(y + 1).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::top, world_position, Chunk->at(x).at(y).at(z).p_BlockType);
							}
						}
					}
				}
			}
		}

		// Upload the data to the GPU whenever the mesh is reconstructed
		p_VBO.BufferSubData(0, this->p_Vertices.size() * sizeof(Vertex),
			&this->p_Vertices.front());
	}

	void ChunkMesh::AddFace(BlockFaceType face_type, const glm::vec3& position, BlockType type)
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

		Vertex v1, v2, v3, v4, v5, v6;

		switch (face_type)
		{
			case BlockFaceType::top :
			{
				v1.position = translation * m_TopFace[0];
				v2.position = translation * m_TopFace[1];
				v3.position = translation * m_TopFace[2];
				v4.position = translation * m_TopFace[2];
				v5.position = translation * m_TopFace[3];
				v6.position = translation * m_TopFace[0];

				break;
			}

			case BlockFaceType::bottom:
			{
				v1.position = translation * m_BottomFace[0];
				v2.position = translation * m_BottomFace[1];
				v3.position = translation * m_BottomFace[2];
				v4.position = translation * m_BottomFace[2];
				v5.position = translation * m_BottomFace[3];
				v6.position = translation * m_BottomFace[0];

				break;
			}

			case BlockFaceType::front:
			{
				v1.position = translation * m_ForwardFace[0];
				v2.position = translation * m_ForwardFace[1];
				v3.position = translation * m_ForwardFace[2];
				v4.position = translation * m_ForwardFace[2];
				v5.position = translation * m_ForwardFace[3];
				v6.position = translation * m_ForwardFace[0];

				break;
			}

			case BlockFaceType::backward :
			{
				v1.position = translation * m_BackFace[0];
				v2.position = translation * m_BackFace[1];
				v3.position = translation * m_BackFace[2];
				v4.position = translation * m_BackFace[2];
				v5.position = translation * m_BackFace[3];
				v6.position = translation * m_BackFace[0];

				break;
			}

			case BlockFaceType::left :
			{
				v1.position = translation * m_LeftFace[0];
				v2.position = translation * m_LeftFace[1];
				v3.position = translation * m_LeftFace[2];
				v4.position = translation * m_LeftFace[2];
				v5.position = translation * m_LeftFace[3];
				v6.position = translation * m_LeftFace[0];

				break;
			}

			case BlockFaceType::right :
			{
				v1.position = translation * m_RightFace[0];
				v2.position = translation * m_RightFace[1];
				v3.position = translation * m_RightFace[2];
				v4.position = translation * m_RightFace[2];
				v5.position = translation * m_RightFace[3];
				v6.position = translation * m_RightFace[0];

				break;
			}

			default :
			{
				// Todo : Throw an error here
				break;
			}
		}

		// For now, set generic texture coordinates

		//std::array<GLfloat, 8> TextureCoordinates = { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		std::array<GLfloat, 8> TextureCoordinates = GetBlockTexture(type, face_type);

		/*v1.texture_coords = glm::vec2(1.0f, 0.0f);
		v2.texture_coords = glm::vec2(1.0f, 1.0f);
		v3.texture_coords = glm::vec2(0.0f, 1.0f);
		v4.texture_coords = glm::vec2(0.0f, 1.0f);
		v5.texture_coords = glm::vec2(0.0f, 0.0f);
		v6.texture_coords = glm::vec2(1.0f, 0.0f);*/

		v1.texture_coords = glm::vec2(TextureCoordinates[0], TextureCoordinates[1]);
		v2.texture_coords = glm::vec2(TextureCoordinates[2], TextureCoordinates[3]);
		v3.texture_coords = glm::vec2(TextureCoordinates[4], TextureCoordinates[5]);
		v4.texture_coords = glm::vec2(TextureCoordinates[4], TextureCoordinates[5]);
		v5.texture_coords = glm::vec2(TextureCoordinates[6], TextureCoordinates[7]);
		v6.texture_coords = glm::vec2(TextureCoordinates[0], TextureCoordinates[1]);

		// Push the vertices to the buffer
		p_Vertices.push_back(v1);
		p_Vertices.push_back(v2);
		p_Vertices.push_back(v3);
		p_Vertices.push_back(v4);
		p_Vertices.push_back(v5);
		p_Vertices.push_back(v6);
	}
}