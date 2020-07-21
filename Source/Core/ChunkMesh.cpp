#include "ChunkMesh.h"

namespace Minecraft
{
	ChunkMesh::ChunkMesh() : p_VBO(GL_ARRAY_BUFFER)
	{
		static bool IndexBufferInitialized = false;

		// Static index buffer
		static GLClasses::IndexBuffer StaticIBO;

		if (IndexBufferInitialized == false)
		{
			IndexBufferInitialized = true;

			GLuint* IndexBuffer = nullptr;

			int index_size = ChunkSizeX * ChunkSizeY * ChunkSizeZ * 6;
			int index_offset = 0;

			IndexBuffer = new GLuint[index_size * 6];

			for (size_t i = 0; i < index_size; i += 6)
			{
				IndexBuffer[i] = 0 + index_offset;
				IndexBuffer[i + 1] = 1 + index_offset;
				IndexBuffer[i + 2] = 2 + index_offset;
				IndexBuffer[i + 3] = 2 + index_offset;
				IndexBuffer[i + 4] = 3 + index_offset;
				IndexBuffer[i + 5] = 0 + index_offset;

				index_offset = index_offset + 4;
			}

			StaticIBO.BufferData(index_size * 6 * sizeof(GLuint), IndexBuffer, GL_STATIC_DRAW);

			delete[] IndexBuffer;
		}

		int stride = (6 * sizeof(GLfloat));

		p_VAO.Bind();
		p_VBO.Bind();
		StaticIBO.Bind();
		p_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		p_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		p_VBO.VertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
		p_VAO.Unbind();

		// Set the values of the 2D planes

		m_ForwardFace[0] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		m_ForwardFace[1] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		m_ForwardFace[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_ForwardFace[3] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

		m_BackFace[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		m_BackFace[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		m_BackFace[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_BackFace[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		m_TopFace[0] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		m_TopFace[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_TopFace[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_TopFace[3] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

		m_BottomFace[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		m_BottomFace[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		m_BottomFace[2] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		m_BottomFace[3] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		
		m_LeftFace[0] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
		m_LeftFace[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		m_LeftFace[2] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		m_LeftFace[3] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		m_RightFace[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_RightFace[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_RightFace[2] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		m_RightFace[3] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	}

	ChunkMesh::~ChunkMesh()
	{
		m_Vertices.clear();
	}

	// Construct mesh using greedy meshing for maximum performance
	void ChunkMesh::ConstructMesh(ChunkDataTypePtr ChunkData, const glm::vec3& chunk_pos)
	{
		glm::vec3 world_position;
		m_Vertices.clear();

		for (int x = 0; x < ChunkSizeX; x++)
		{
			for (int y = 0; y < ChunkSizeY; y++)
			{
				for (int z = 0; z < ChunkSizeZ; z++)
				{
					if (ChunkData->at(x).at(y).at(z).p_BlockType != BlockType::Air)
					{
						// To fix chunk edge mesh building issues, both faces are added if it is in the edge

						world_position.x = chunk_pos.x * ChunkSizeX + x;
						world_position.y = 0 * ChunkSizeY + y;
						world_position.z = chunk_pos.z * ChunkSizeZ + z;

						if (z <= 0)
						{
							AddFace(BlockFaceType::backward, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							AddFace(BlockFaceType::front, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));

						}

						else if (z >= ChunkSizeZ - 1)
						{
							AddFace(BlockFaceType::front, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							AddFace(BlockFaceType::backward, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
						}

						else
						{
							//If the forward block is an air block, add the forward face to the mesh
							if (ChunkData->at(x).at(y).at(z + 1).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::front, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							}

							// If the back (-forward) block is an air block, add the back face to the mesh
							if (ChunkData->at(x).at(y).at(z - 1).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::backward, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							}
						}

						if (x <= 0)
						{
							AddFace(BlockFaceType::left, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							AddFace(BlockFaceType::right, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));

						}

						else if (x >= ChunkSizeX - 1)
						{
							AddFace(BlockFaceType::right, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							AddFace(BlockFaceType::left, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));

						}

						else
						{
							// If the next block is an air block, add the right face to the mesh
							if (ChunkData->at(x + 1).at(y).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::right, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							}

							// If the previous block is an air block, add the left face to the mesh
							if (ChunkData->at(x - 1).at(y).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::left, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							}
						}

						if (y <= 0)
						{
							AddFace(BlockFaceType::bottom, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							AddFace(BlockFaceType::top, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));

						}

						else if (y >= ChunkSizeY - 1)
						{
							AddFace(BlockFaceType::top, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							AddFace(BlockFaceType::bottom, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
						}

						else
						{
							// If the top block is an air block, add the top face to the mesh
							if (ChunkData->at(x).at(y - 1).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::bottom, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							}

							// If the bottom block is an air block, add the top face to the mesh
							if (ChunkData->at(x).at(y + 1).at(z).p_BlockType == BlockType::Air)
							{
								AddFace(BlockFaceType::top, world_position, static_cast<BlockType>(ChunkData->at(x).at(y).at(z).p_BlockType));
							}
						}
					}
				}
			}
		}

		// Upload the data to the GPU whenever the mesh is reconstructed

		p_VBO.BufferData(this->m_Vertices.size() * sizeof(Vertex), &this->m_Vertices.front(), GL_STATIC_DRAW);
		p_VerticesCount = m_Vertices.size();
		m_Vertices.clear();
	}

	void ChunkMesh::AddFace(BlockFaceType face_type, const glm::vec3& position, BlockType type)
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

		Vertex v1, v2, v3, v4, v5, v6;

		// Order
		// Top, bottom, front, back, left, right
		static const GLfloat lighting_levels[6] = { 1.0f, 0.2f, 0.6f, 0.7f, 0.6f, 0.7f };

		switch (face_type)
		{
			case BlockFaceType::top :
			{
				v1.position = translation * m_TopFace[0];
				v2.position = translation * m_TopFace[1];
				v3.position = translation * m_TopFace[2];
				v4.position = translation * m_TopFace[3];
				
				// Set the lighting level for the vertex
				v1.lighting_level = lighting_levels[0];
				v2.lighting_level = lighting_levels[0];
				v3.lighting_level = lighting_levels[0];
				v4.lighting_level = lighting_levels[0];

				break;
			}

			case BlockFaceType::bottom:
			{
				v1.position = translation * m_BottomFace[0];
				v2.position = translation * m_BottomFace[1];
				v3.position = translation * m_BottomFace[2];
				v4.position = translation * m_BottomFace[3];

				// Set the lighting level for the vertex
				v1.lighting_level = lighting_levels[1];
				v2.lighting_level = lighting_levels[1];
				v3.lighting_level = lighting_levels[1];
				v4.lighting_level = lighting_levels[1];

				break;
			}

			case BlockFaceType::front:
			{
				v1.position = translation * m_ForwardFace[0];
				v2.position = translation * m_ForwardFace[1];
				v3.position = translation * m_ForwardFace[2];
				v4.position = translation * m_ForwardFace[3];

				// Set the lighting level for the vertex
				v1.lighting_level = lighting_levels[2];
				v2.lighting_level = lighting_levels[2];
				v3.lighting_level = lighting_levels[2];
				v4.lighting_level = lighting_levels[2];

				break;
			}

			case BlockFaceType::backward :
			{
				v1.position = translation * m_BackFace[0];
				v2.position = translation * m_BackFace[1];
				v3.position = translation * m_BackFace[2];
				v4.position = translation * m_BackFace[3];

				v1.lighting_level = lighting_levels[3];
				v2.lighting_level = lighting_levels[3];
				v3.lighting_level = lighting_levels[3];
				v4.lighting_level = lighting_levels[3];

				break;
			}

			case BlockFaceType::left :
			{
				v1.position = translation * m_LeftFace[0];
				v2.position = translation * m_LeftFace[1];
				v3.position = translation * m_LeftFace[2];
				v4.position = translation * m_LeftFace[3];

				v1.lighting_level = lighting_levels[4];
				v2.lighting_level = lighting_levels[4];
				v3.lighting_level = lighting_levels[4];
				v4.lighting_level = lighting_levels[4];

				break;
			}

			case BlockFaceType::right :
			{
				v1.position = translation * m_RightFace[0];
				v2.position = translation * m_RightFace[1];
				v3.position = translation * m_RightFace[2];
				v4.position = translation * m_RightFace[3];

				v1.lighting_level = lighting_levels[5];
				v2.lighting_level = lighting_levels[5];
				v3.lighting_level = lighting_levels[5];
				v4.lighting_level = lighting_levels[5];

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

		v1.texture_coords = glm::vec2(TextureCoordinates[0], TextureCoordinates[1]);
		v2.texture_coords = glm::vec2(TextureCoordinates[2], TextureCoordinates[3]);
		v3.texture_coords = glm::vec2(TextureCoordinates[4], TextureCoordinates[5]);
		v4.texture_coords = glm::vec2(TextureCoordinates[6], TextureCoordinates[7]);

		// Push the vertices to the buffer
		m_Vertices.push_back(v1);
		m_Vertices.push_back(v2);
		m_Vertices.push_back(v3);
		m_Vertices.push_back(v4);
	}
}