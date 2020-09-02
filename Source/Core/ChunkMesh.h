#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <array>

#include "OpenGL Classes/VertexBuffer.h"
#include "OpenGL Classes/IndexBuffer.h"
#include "OpenGL Classes/VertexArray.h"
#include "OpenGL Classes/Shader.h"
#include "OpenGL Classes/Texture.h"

#include "Models/Model.h"

//#include "Utils/Vertex.h"
#include "BlockDatabase.h"

namespace Minecraft
{
	class Chunk;
	typedef std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>* ChunkDataTypePtr;
	typedef std::array<std::array<std::array<uint8_t, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>* ChunkLightDataTypePtr;

	// Forward declarations
	ChunkDataTypePtr _GetChunkDataForMeshing(int cx, int cz);
	ChunkLightDataTypePtr _GetChunkLightDataForMeshing(int cx, int cz);

	class ChunkMesh
	{
	public : 

		ChunkMesh();
		~ChunkMesh();

		void ConstructMesh(Chunk* chunk, const glm::vec3& chunk_pos);
		
		std::uint32_t p_VerticesCount;
		std::uint32_t p_TransparentVerticesCount;
		std::uint32_t p_ModelVerticesCount;
		GLClasses::VertexArray p_VAO;
		GLClasses::VertexArray p_TransparentVAO;
		GLClasses::VertexArray p_ModelVAO;

	private : 

		void AddFace(Chunk* chunk, BlockFaceType face_type, const glm::vec3& position, BlockType type, uint8_t light_level,
			bool buffer = true);

		void AddModel(Chunk* chunk, const glm::vec3& local_pos, BlockType type, float light_level);

		std::vector<Vertex> m_Vertices;
		std::vector<Vertex> m_TransparentVertices;
		std::vector<Vertex> m_ModelVertices;
		glm::vec4 m_TopFace[4];
		glm::vec4 m_BottomFace[4];
		glm::vec4 m_ForwardFace[4];
		glm::vec4 m_BackFace[4];
		glm::vec4 m_LeftFace[4];
		glm::vec4 m_RightFace[4];

		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexBuffer m_TransparentVBO; // Vertex buffer for trasparent blocks
		GLClasses::VertexBuffer m_ModelVBO; // Vertex buffer for trasparent blocks
	};
}
