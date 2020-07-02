#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL Classes/VertexBuffer.h"
#include "OpenGL Classes/VertexArray.h"
#include "OpenGL Classes/IndexBuffer.h"
#include "OpenGL Classes/Shader.h"
#include "OpenGL Classes/Texture.h"

namespace Minecraft
{
	using namespace GLClasses;

	struct Cube
	{
		float length;
		float height;
		float breadth;
	};

	class CubeRenderer
	{
	public :
		CubeRenderer();
		~CubeRenderer();
		void RenderCube(const glm::vec3& position, Texture* texture, float rotation, const glm::mat4& projection, const glm::mat4& view = glm::mat4(1.0f), Shader* shader = nullptr);

	private :

		GLClasses::VertexBuffer m_VBO;
		GLClasses::IndexBuffer m_IBO;
		GLClasses::VertexArray m_VAO;
		GLClasses::Shader m_DefaultShader;
	};
}
