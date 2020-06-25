#include "ChunkMesh.h"

namespace Minecraft
{
	ChunkMesh::ChunkMesh()
	{
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

	void ChunkMesh::ConstructMesh(std::array<std::array<std::array<Block, ChunkSizeX>, ChunkSizeY>, ChunkSizeZ>* Chunk)
	{
		for (int x = 0; x < Chunk->size(); x++)
		{
			for (int y = 0; y < Chunk->at(x).size(); y++)
			{
				for (int z = 0; z < Chunk->at(x).at(y).size(); z++)
				{
					// If the next block is an air block, add the right face to the mesh
					if (Chunk->at(x + 1).at(y).at(z).p_BlockType == BlockType::Air)
					{
						AddFace(FaceType::right, Chunk->at(x).at(y).at(z).p_Position);
					}

					// If the previous block is an air block, add the left face to the mesh
					if (Chunk->at(x - 1).at(y).at(z).p_BlockType == BlockType::Air)
					{
						AddFace(FaceType::left, Chunk->at(x).at(y).at(z).p_Position);
					}

					// If the top block is an air block, add the top face to the mesh
					if (Chunk->at(x).at(y - 1).at(z).p_BlockType == BlockType::Air)
					{
						AddFace(FaceType::top, Chunk->at(x).at(y).at(z).p_Position);
					}

					// If the bottom block is an air block, add the top face to the mesh
					if (Chunk->at(x).at(y + 1).at(z).p_BlockType == BlockType::Air)
					{
						AddFace(FaceType::bottom, Chunk->at(x).at(y).at(z).p_Position);
					}

					// If the forward block is an air block, add the forward face to the mesh
					if (Chunk->at(x).at(y).at(z + 1).p_BlockType == BlockType::Air)
					{
						AddFace(FaceType::forward, Chunk->at(x).at(y).at(z).p_Position);
					}

					// If the back (-forward) block is an air block, add the back face to the mesh
					if (Chunk->at(x).at(y).at(z - 1).p_BlockType == BlockType::Air)
					{
						AddFace(FaceType::backward, Chunk->at(x).at(y).at(z).p_Position);
					}
				}
			}
		}
	}

	void ChunkMesh::AddFace(FaceType face_type, const glm::vec3& position)
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

		Vertex v1, v2, v3, v4, v5, v6;

		switch (face_type)
		{
			case FaceType::top : 
			{
				v1.position = translation * m_TopFace[0];
				v2.position = translation * m_TopFace[1];
				v3.position = translation * m_TopFace[2];
				v4.position = translation * m_TopFace[2];
				v5.position = translation * m_TopFace[3];
				v6.position = translation * m_TopFace[0];

				break;
			}

			case FaceType::bottom:
			{
				v1.position = translation * m_BottomFace[0];
				v2.position = translation * m_BottomFace[1];
				v3.position = translation * m_BottomFace[2];
				v4.position = translation * m_BottomFace[2];
				v5.position = translation * m_BottomFace[3];
				v6.position = translation * m_BottomFace[0];

				break;
			}

			case FaceType::forward :
			{
				v1.position = translation * m_ForwardFace[0];
				v2.position = translation * m_ForwardFace[1];
				v3.position = translation * m_ForwardFace[2];
				v4.position = translation * m_ForwardFace[2];
				v5.position = translation * m_ForwardFace[3];
				v6.position = translation * m_ForwardFace[0];

				break;
			}

			case FaceType::backward : 
			{
				v1.position = translation * m_BackFace[0];
				v2.position = translation * m_BackFace[1];
				v3.position = translation * m_BackFace[2];
				v4.position = translation * m_BackFace[2];
				v5.position = translation * m_BackFace[3];
				v6.position = translation * m_BackFace[0];

				break;
			}

			case FaceType::left : 
			{
				v1.position = translation * m_LeftFace[0];
				v2.position = translation * m_LeftFace[1];
				v3.position = translation * m_LeftFace[2];
				v4.position = translation * m_LeftFace[2];
				v5.position = translation * m_LeftFace[3];
				v6.position = translation * m_LeftFace[0];

				break;
			}

			case FaceType::right : 
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

		v1.texture_coords = glm::vec2(1.0f, 0.0f);
		v2.texture_coords = glm::vec2(1.0f, 1.0f);
		v3.texture_coords = glm::vec2(0.0f, 1.0f);
		v4.texture_coords = glm::vec2(0.0f, 1.0f);
		v5.texture_coords = glm::vec2(0.0f, 0.0f);
		v6.texture_coords = glm::vec2(1.0f, 0.0f);

		// Push the vertices to the buffer
		p_Vertices.push_back(v1);
		p_Vertices.push_back(v2);
		p_Vertices.push_back(v3);
		p_Vertices.push_back(v4);
		p_Vertices.push_back(v5);
		p_Vertices.push_back(v6);
	}
}