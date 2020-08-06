#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/Texture.h"
#include "../OpenGL Classes/GLDebug.h"

#include "../Chunk.h"
#include "../FpsCamera.h"

namespace Minecraft
{
	class Renderer
	{
	public : 

		Renderer();
		void RenderChunk(Chunk* chunk);
		void StartChunkRendering(FPSCamera* camera, const glm::vec4& ambient_light, int render_distance);
		void EndChunkRendering();

	private: 
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexArray m_VAO;
		GLClasses::Shader m_DefaultShader;
		GLClasses::Texture m_BlockAtlas;
	};
}