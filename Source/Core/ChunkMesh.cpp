#include "ChunkMesh.h"
#include "Chunk.h"


/*
		-- The Chunk Meshing Process --

-- Normal blocks --
This algorithm iterates throught every block in a chunk, 
if the left block is an air block, it adds that faces to the mesh, same for the other surrounding blocks;

-- Transparent blocks --
This mesher adds all transparent blocks of the same type together. eg : a group of water faces or leaf faces. 
What we are left with is like a shell of that transparent block which is quite efficient.

-- Meshes --
There are 3 meshes per chunk that are all using std::vector as it is fast and efficient. The buffer is cleared after it is uploaded to the gpu.
Meshes : 
- Normal block mesh
- Transparent block mesh
- Model mesh 

-- Shadows -- 
A basic algorithm is used to calculate shadows. It checks the highest block in a chunk and sets the shadow level if it is a particular range

-- Lighting -- 
I retrieve the light value from the 3d light value array in a chunk and store it in each vertex

-- Info --
When ever a chunk is updated. The entire mesh is regenerated instead of modifying the existing vertices..
Index buffers are used to maximize performance

The _GetchunkDataForMeshing() functions are forward declarations and are implemented in main.cpp 
*/

namespace Minecraft
{
	ChunkMesh::ChunkMesh() : m_VBO(GL_ARRAY_BUFFER), m_TransparentVBO(GL_ARRAY_BUFFER), m_ModelVBO(GL_ARRAY_BUFFER)
	{
		static bool IndexBufferInitialized = false;

		// Static index buffer
		static GLClasses::IndexBuffer StaticIBO;

		if (IndexBufferInitialized == false)
		{
			IndexBufferInitialized = true;

			GLuint* IndexBuffer = nullptr;

			int index_size = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 6;
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

		p_VAO.Bind();
		m_VBO.Bind();
		StaticIBO.Bind();
		m_VBO.VertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		m_VBO.VertexAttribIPointer(1, 2, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));
		m_VBO.VertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, lighting_level));
		m_VBO.VertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, block_face_lighting));
		p_VAO.Unbind();

		p_TransparentVAO.Bind();
		m_TransparentVBO.Bind();
		StaticIBO.Bind();
		m_TransparentVBO.VertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		m_TransparentVBO.VertexAttribIPointer(1, 2, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));
		m_TransparentVBO.VertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, lighting_level));
		m_TransparentVBO.VertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, block_face_lighting));
		p_TransparentVAO.Unbind();

		p_ModelVAO.Bind();
		m_ModelVBO.Bind();
		StaticIBO.Bind();
		m_ModelVBO.VertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		m_ModelVBO.VertexAttribIPointer(1, 2, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));
		m_ModelVBO.VertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, lighting_level));
		m_ModelVBO.VertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, block_face_lighting));
		p_ModelVAO.Unbind();

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
	bool ChunkMesh::ConstructMesh(Chunk* chunk, const glm::vec3& chunk_pos)
	{
		ChunkDataTypePtr ChunkData = &chunk->p_ChunkContents;
		ChunkLightDataTypePtr ChunkLData = &chunk->p_ChunkLightInformation;

		glm::vec3 world_position;
		glm::vec3 local_position;
		m_Vertices.clear();

		ChunkDataTypePtr ForwardChunkData = _GetChunkDataForMeshing(static_cast<int>(chunk_pos.x), static_cast<int>(chunk_pos.z + 1));
		ChunkDataTypePtr BackwardChunkData = _GetChunkDataForMeshing(static_cast<int>(chunk_pos.x), static_cast<int>(chunk_pos.z - 1));
		ChunkDataTypePtr RightChunkData = _GetChunkDataForMeshing(static_cast<int>(chunk_pos.x + 1), static_cast<int>(chunk_pos.z));
		ChunkDataTypePtr LeftChunkData = _GetChunkDataForMeshing(static_cast<int>(chunk_pos.x - 1), static_cast<int>(chunk_pos.z));
		ChunkLightDataTypePtr ForwardChunkLData = _GetChunkLightDataForMeshing(static_cast<int>(chunk_pos.x), static_cast<int>(chunk_pos.z + 1));
		ChunkLightDataTypePtr BackwardChunkLData = _GetChunkLightDataForMeshing(static_cast<int>(chunk_pos.x), static_cast<int>(chunk_pos.z - 1));
		ChunkLightDataTypePtr RightChunkLData = _GetChunkLightDataForMeshing(static_cast<int>(chunk_pos.x + 1), static_cast<int>(chunk_pos.z));
		ChunkLightDataTypePtr LeftChunkLData = _GetChunkLightDataForMeshing(static_cast<int>(chunk_pos.x - 1), static_cast<int>(chunk_pos.z));

		if (ForwardChunkData && BackwardChunkData && RightChunkData && LeftChunkData)
		{
			for (int x = 0; x < CHUNK_SIZE_X; x++)
			{
				for (int y = 0; y < CHUNK_SIZE_Y; y++)
				{
					for (int z = 0; z < CHUNK_SIZE_Z; z++)
					{
						if (ChunkData->at(x).at(y).at(z).p_BlockType != BlockType::Air)
						{
							Block* block = &ChunkData->at(x).at(y).at(z);

							// To fix chunk edge mesh building issues, both faces are added if it is in the edge

							float light_level = ChunkLData->at(x).at(y).at(z);

							if (y >= 0 && y < CHUNK_SIZE_Y - 1)
							{
								light_level = ChunkLData->at(x).at(y + 1).at(z);
							}

							world_position.x = chunk_pos.x * CHUNK_SIZE_X + x;
							world_position.y = 0 * CHUNK_SIZE_Y + y;
							world_position.z = chunk_pos.z * CHUNK_SIZE_Z + z;
							local_position = glm::vec3(x, y, z);

							if (block->IsModel())
							{
								AddModel(chunk, local_position, block->p_BlockType, light_level);
								continue;
							}

							if (z <= 0)
							{
								if (block->IsTransparent())
								{
									if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsTransparent() &&
										BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).p_BlockType != block->p_BlockType)
									{
										light_level = BackwardChunkLData->at(x).at(y).at(CHUNK_SIZE_Z - 1);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level, false);
									}

									else if (ChunkData->at(x).at(y).at(1).IsTransparent() &&
										ChunkData->at(x).at(y).at(1).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x).at(y).at(1);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level, false);
									}
								}

								else
								{
									if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsOpaque() == false)
									{
										light_level = BackwardChunkLData->at(x).at(y).at(CHUNK_SIZE_Z - 1);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level);
									}

									else if (ChunkData->at(x).at(y).at(1).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x).at(y).at(1);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level);
									}
								}
							}

							else if (z >= CHUNK_SIZE_Z - 1)
							{
								if (block->IsTransparent())
								{
									if (ForwardChunkData->at(x).at(y).at(0).IsTransparent() &&
										ForwardChunkData->at(x).at(y).at(0).p_BlockType != block->p_BlockType)
									{
										light_level = ForwardChunkLData->at(x).at(y).at(0);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level, false);
									}

									else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsTransparent() &&
										ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x).at(y).at(CHUNK_SIZE_Z - 2);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level, false);
									}
								}

								else
								{
									if (ForwardChunkData->at(x).at(y).at(0).IsOpaque() == false)
									{
										light_level = ForwardChunkLData->at(x).at(y).at(0);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level);
									}

									else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x).at(y).at(CHUNK_SIZE_Z - 2);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level);
									}
								}
							}

							else
							{
								if (block->IsTransparent())
								{
									if (ChunkData->at(x).at(y).at(z + 1).IsTransparent() &&
										ChunkData->at(x).at(y).at(z + 1).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x).at(y).at(z + 1);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level, false);
									}

									if (ChunkData->at(x).at(y).at(z - 1).IsTransparent() &&
										ChunkData->at(x).at(y).at(z - 1).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x).at(y).at(z - 1);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level, false);
									}
								}

								else
								{
									//If the forward block is an air block, add the forward face to the mesh
									if (ChunkData->at(x).at(y).at(z + 1).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x).at(y).at(z + 1);
										AddFace(chunk, BlockFaceType::front, local_position, block->p_BlockType, light_level);
									}

									// If the back (-forward) block is an air block, add the back face to the mesh
									if (ChunkData->at(x).at(y).at(z - 1).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x).at(y).at(z - 1);
										AddFace(chunk, BlockFaceType::backward, local_position, block->p_BlockType, light_level);
									}
								}
							}

							if (x <= 0)
							{
								if (block->IsTransparent())
								{
									if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsTransparent() &&
										LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = LeftChunkLData->at(CHUNK_SIZE_X - 1).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level, false);
									}

									else if (ChunkData->at(1).at(y).at(z).IsTransparent() &&
										ChunkData->at(1).at(y).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(1).at(y).at(z);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level, false);
									}

								}

								else
								{
									if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsOpaque() == false)
									{
										light_level = LeftChunkLData->at(CHUNK_SIZE_X - 1).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level);
									}

									else if (ChunkData->at(1).at(y).at(z).IsOpaque() == false)
									{
										light_level = ChunkLData->at(1).at(y).at(z);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level);
									}
								}
							}

							else if (x >= CHUNK_SIZE_X - 1)
							{
								if (block->IsTransparent())
								{
									if (RightChunkData->at(0).at(y).at(z).IsTransparent() &&
										RightChunkData->at(0).at(y).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = RightChunkLData->at(0).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level, false);
									}

									else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsTransparent() &&
										ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(CHUNK_SIZE_X - 2).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level, false);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level, false);
									}
								}

								else
								{
									if (RightChunkData->at(0).at(y).at(z).IsOpaque() == false)
									{
										light_level = RightChunkLData->at(0).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level);
									}

									else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsOpaque() == false)
									{
										light_level = ChunkLData->at(CHUNK_SIZE_X - 2).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level);
									}
								}

							}

							else
							{
								if (block->IsTransparent())
								{
									if (ChunkData->at(x + 1).at(y).at(z).IsTransparent() &&
										ChunkData->at(x + 1).at(y).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x + 1).at(y).at(z);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level, false);
									}

									if (ChunkData->at(x - 1).at(y).at(z).IsTransparent() &&
										ChunkData->at(x - 1).at(y).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x - 1).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level, false);
									}
								}

								else
								{
									// If the next block is an air block, add the right face to the mesh
									if (ChunkData->at(x + 1).at(y).at(z).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x + 1).at(y).at(z);
										AddFace(chunk, BlockFaceType::right, local_position, block->p_BlockType, light_level);
									}

									// If the previous block is an air block, add the left face to the mesh
									if (ChunkData->at(x - 1).at(y).at(z).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x - 1).at(y).at(z);
										AddFace(chunk, BlockFaceType::left, local_position, block->p_BlockType, light_level);
									}
								}
							}

							if (y <= 0)
							{
								if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFaceType::bottom, local_position, block->p_BlockType, light_level);
								}
							}

							else if (y >= CHUNK_SIZE_Y - 1)
							{
								AddFace(chunk, BlockFaceType::top, local_position, block->p_BlockType, light_level);
							}

							else
							{
								if (block->IsTransparent())
								{
									if (ChunkData->at(x).at(y - 1).at(z).IsTransparent() &&
										ChunkData->at(x).at(y - 1).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x).at(y - 1).at(z);
										AddFace(chunk, BlockFaceType::bottom, local_position, block->p_BlockType, light_level, false);
									}

									if (ChunkData->at(x).at(y + 1).at(z).IsTransparent() &&
										ChunkData->at(x).at(y + 1).at(z).p_BlockType != block->p_BlockType)
									{
										light_level = ChunkLData->at(x).at(y + 1).at(z);
										AddFace(chunk, BlockFaceType::top, local_position, block->p_BlockType, light_level, false);
									}
								}

								else
								{
									// If the top block is an air block, add the top face to the mesh
									if (ChunkData->at(x).at(y - 1).at(z).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x).at(y - 1).at(z);
										AddFace(chunk, BlockFaceType::bottom, local_position, block->p_BlockType, light_level);
									}

									// If the bottom block is an air block, add the top face to the mesh
									if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
									{
										light_level = ChunkLData->at(x).at(y + 1).at(z);
										AddFace(chunk, BlockFaceType::top, local_position, block->p_BlockType, light_level);
									}
								}
							}
						}
					}
				}
			}

			// Upload the data to the GPU whenever the mesh is reconstructed

			p_VerticesCount = 0;
			p_TransparentVerticesCount = 0;
			p_ModelVerticesCount = 0;

			if (m_Vertices.size() > 0)
			{
				m_VBO.BufferData(this->m_Vertices.size() * sizeof(Vertex), &this->m_Vertices.front(), GL_STATIC_DRAW);
				p_VerticesCount = m_Vertices.size();
				m_Vertices.clear();
			}

			if (m_TransparentVertices.size() > 0)
			{
				m_TransparentVBO.BufferData(this->m_TransparentVertices.size() * sizeof(Vertex), &this->m_TransparentVertices.front(), GL_STATIC_DRAW);
				p_TransparentVerticesCount = m_TransparentVertices.size();
				m_TransparentVertices.clear();
			}

			if (m_ModelVertices.size() > 0)
			{
				m_ModelVBO.BufferData(this->m_ModelVertices.size() * sizeof(Vertex), &this->m_ModelVertices.front(), GL_STATIC_DRAW);
				p_ModelVerticesCount = m_ModelVertices.size();
				m_ModelVertices.clear();
			}

			return true;
		}

		return false;
	}

	glm::ivec3 ConvertWorldPosToBlock(const glm::vec3& position)
	{
		int block_chunk_x = static_cast<int>(floor(position.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(position.z / CHUNK_SIZE_Z));
		int lx = position.x - (block_chunk_x * CHUNK_SIZE_X);
		int ly = static_cast<int>(floor(position.y));
		int lz = position.z - (block_chunk_z * CHUNK_SIZE_Z);

		return glm::ivec3(lx, ly, lz);
	}

	bool HasShadow(Chunk* chunk, int x, int y, int z)
	{
		constexpr int max_shadow = 24;
		for (int i = y + 1; i < y + max_shadow; i++)
		{
			if (i < CHUNK_SIZE_Y)
			{
				if (chunk->p_ChunkContents.at(x).at(i).at(z).CastsShadow())
				{
					return true;
				}
			}
		}

		return false;
	}

	void ChunkMesh::AddFace(Chunk* chunk, BlockFaceType face_type, const glm::vec3& position, BlockType type, uint8_t light_level,
		bool buffer)
	{
		glm::vec4 translation = glm::vec4(position, 0.0f); // No need to create a model matrix. 
		// Adding the position to the translation will do the samething but much much faster

		Vertex v1, v2, v3, v4;

		// To fix a face culling issue. I inverted the vertices for the left, front and bottom face so the winding order will be correct
		bool reverse_texture_coordinates = false;

		// Order
		// Top, bottom, front, back, left, right
		static const uint8_t lighting_levels[6] = { 10, 3, 6, 7, 6, 7 };

		switch (face_type)
		{
		case BlockFaceType::top:
		{
			uint8_t face_light_level = 10;

			if (HasShadow(chunk, position.x, position.y, position.z))
			{
				face_light_level -= 2;
			}

			v1.position = translation + m_TopFace[0];
			v2.position = translation + m_TopFace[1];
			v3.position = translation + m_TopFace[2];
			v4.position = translation + m_TopFace[3];

			// Set the lighting level for the vertex
			v1.lighting_level = light_level;
			v2.lighting_level = light_level;
			v3.lighting_level = light_level;
			v4.lighting_level = light_level;

			v1.block_face_lighting = face_light_level;
			v2.block_face_lighting = face_light_level;
			v3.block_face_lighting = face_light_level;
			v4.block_face_lighting = face_light_level;

			break;
		}

		case BlockFaceType::bottom:
		{
			v1.position = translation + m_BottomFace[3];
			v2.position = translation + m_BottomFace[2];
			v3.position = translation + m_BottomFace[1];
			v4.position = translation + m_BottomFace[0];

			// Set the lighting level for the vertex
			v1.lighting_level = light_level;
			v2.lighting_level = light_level;
			v3.lighting_level = light_level;
			v4.lighting_level = light_level;

			v1.block_face_lighting = lighting_levels[1];
			v2.block_face_lighting = lighting_levels[1];
			v3.block_face_lighting = lighting_levels[1];
			v4.block_face_lighting = lighting_levels[1];

			reverse_texture_coordinates = true;

			break;
		}

		case BlockFaceType::front:
		{
			v1.position = translation + m_ForwardFace[3];
			v2.position = translation + m_ForwardFace[2];
			v3.position = translation + m_ForwardFace[1];
			v4.position = translation + m_ForwardFace[0];

			// Set the lighting level for the vertex
			v1.lighting_level = light_level;
			v2.lighting_level = light_level;
			v3.lighting_level = light_level;
			v4.lighting_level = light_level;

			v1.block_face_lighting = lighting_levels[2];
			v2.block_face_lighting = lighting_levels[2];
			v3.block_face_lighting = lighting_levels[2];
			v4.block_face_lighting = lighting_levels[2];

			reverse_texture_coordinates = true;

			break;
		}

		case BlockFaceType::backward:
		{
			v1.position = translation + m_BackFace[0];
			v2.position = translation + m_BackFace[1];
			v3.position = translation + m_BackFace[2];
			v4.position = translation + m_BackFace[3];

			v1.lighting_level = light_level;
			v2.lighting_level = light_level;
			v3.lighting_level = light_level;
			v4.lighting_level = light_level;

			v1.block_face_lighting = lighting_levels[3];
			v2.block_face_lighting = lighting_levels[3];
			v3.block_face_lighting = lighting_levels[3];
			v4.block_face_lighting = lighting_levels[3];

			break;
		}

		case BlockFaceType::left:
		{
			v1.position = translation + m_LeftFace[3];
			v2.position = translation + m_LeftFace[2];
			v3.position = translation + m_LeftFace[1];
			v4.position = translation + m_LeftFace[0];

			v1.lighting_level = light_level;
			v2.lighting_level = light_level;
			v3.lighting_level = light_level;
			v4.lighting_level = light_level;

			v1.block_face_lighting = lighting_levels[4];
			v2.block_face_lighting = lighting_levels[4];
			v3.block_face_lighting = lighting_levels[4];
			v4.block_face_lighting = lighting_levels[4];

			reverse_texture_coordinates = true;

			break;
		}

		case BlockFaceType::right:
		{
			v1.position = translation + m_RightFace[0];
			v2.position = translation + m_RightFace[1];
			v3.position = translation + m_RightFace[2];
			v4.position = translation + m_RightFace[3];

			v1.lighting_level = light_level;
			v2.lighting_level = light_level;
			v3.lighting_level = light_level;
			v4.lighting_level = light_level;

			v1.block_face_lighting = lighting_levels[5];
			v2.block_face_lighting = lighting_levels[5];
			v3.block_face_lighting = lighting_levels[5];
			v4.block_face_lighting = lighting_levels[5];

			break;
		}

		default:
		{
			// Todo : Throw an error here
			break;
		}
		}

		if (type == BlockType::Water) {

			v1.block_face_lighting = 85;
			v2.block_face_lighting = 85;
			v3.block_face_lighting = 85;
			v4.block_face_lighting = 85;
		}

		// Get required texture coordinates

		const std::array<uint16_t, 8>& TextureCoordinates = BlockDatabase::GetBlockTexture(type, face_type);

		if (reverse_texture_coordinates)
		{
			v1.texture_coords = { TextureCoordinates[6], TextureCoordinates[7] };
			v2.texture_coords = { TextureCoordinates[4], TextureCoordinates[5] };
			v3.texture_coords = { TextureCoordinates[2], TextureCoordinates[3] };
			v4.texture_coords = { TextureCoordinates[0], TextureCoordinates[1] };
		}

		else
		{
			v1.texture_coords = { TextureCoordinates[0], TextureCoordinates[1] };
			v2.texture_coords = { TextureCoordinates[2], TextureCoordinates[3] };
			v3.texture_coords = { TextureCoordinates[4], TextureCoordinates[5] };
			v4.texture_coords = { TextureCoordinates[6], TextureCoordinates[7] };
		}

		if (buffer)
		{
			m_Vertices.push_back(v1);
			m_Vertices.push_back(v2);
			m_Vertices.push_back(v3);
			m_Vertices.push_back(v4);
		}

		else if (!buffer)
		{
			m_TransparentVertices.push_back(v1);
			m_TransparentVertices.push_back(v2);
			m_TransparentVertices.push_back(v3);
			m_TransparentVertices.push_back(v4);
		}
	}

	// Adds a model such as a flower or a deadbush to the chunk mesh
	void ChunkMesh::AddModel(Chunk* chunk, const glm::vec3& local_pos, BlockType type, float light_level)
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), local_pos);
		Model model(type);

		uint8_t face_light = 10;

		if (HasShadow(chunk, local_pos.x, local_pos.y, local_pos.z))
		{
			face_light -= 2;
		}

		for (int i = 0; i < model.p_ModelVertices.size(); i++)
		{
			Vertex vertex;

			glm::vec4 pos = glm::vec4(model.p_ModelVertices.at(i).position, 1.0f);
			pos = translation * pos;

			vertex.position = glm::vec3(pos.x, pos.y, pos.z);
			vertex.texture_coords = model.p_ModelVertices.at(i).tex_coords;
			vertex.lighting_level = light_level;
			vertex.block_face_lighting = face_light;

			m_ModelVertices.push_back(vertex);
		}
	}
}