#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "OpenGL Classes\VertexBuffer.h"
#include "OpenGL Classes\VertexArray.h"
#include "OpenGL Classes\Shader.h"
#include "OpenGL Classes\Texture.h"

#include "Utils\Vertex.h"
#include "Block.h"

namespace Minecraft
{
	class ChunkMesh
	{
	public : 

		ChunkMesh();
		void ConstructMesh(std::array<std::array<std::array<Block, ChunkSizeX>, ChunkSizeY>, ChunkSizeZ>* Chunk);
		std::vector<Vertex> p_Vertices;

	private : 
		enum FaceType
		{
			top, bottom, left, right, forward, backward
		};

		void AddFace(FaceType face_type, const glm::vec3& position);
		glm::vec4 m_TopFace[4];
		glm::vec4 m_BottomFace[4];
		glm::vec4 m_ForwardFace[4];
		glm::vec4 m_BackFace[4];
		glm::vec4 m_LeftFace[4];
		glm::vec4 m_RightFace[4];
	};
}