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

#include "Utils/Vertex.h"
#include "BlockTextureManager.h"

namespace Minecraft
{
	class ChunkMesh
	{
	public : 

		ChunkMesh();
		~ChunkMesh();

		void ConstructMesh(std::array<std::array<std::array<Block, ChunkSizeX>, ChunkSizeY>, ChunkSizeZ>* Chunk, const glm::vec3& chunk_pos);
		
		std::uint32_t p_VerticesCount;
		GLClasses::VertexBuffer p_VBO;
		GLClasses::VertexArray p_VAO;

	private : 

		void AddFace(BlockFaceType face_type, const glm::vec3& position, BlockType type);
		std::vector<Vertex> m_Vertices;
		glm::vec4 m_TopFace[4];
		glm::vec4 m_BottomFace[4];
		glm::vec4 m_ForwardFace[4];
		glm::vec4 m_BackFace[4];
		glm::vec4 m_LeftFace[4];
		glm::vec4 m_RightFace[4];
	};
}
