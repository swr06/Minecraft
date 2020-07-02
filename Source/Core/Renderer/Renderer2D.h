#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <array>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../OpenGL Classes/IndexBuffer.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/Texture.h"

#include "../OrthographicCamera.h"

namespace Minecraft
{
	class Renderer2D
	{
	public:

		Renderer2D();
	
		void RenderQuad(const glm::vec2& position, GLClasses::Texture* texture, OrthographicCamera* camera);

	private : 

		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexArray m_VAO;
		GLClasses::IndexBuffer m_IBO;
		GLClasses::Shader m_DefaultShader;
	};
}