#include "Renderer.h"

namespace Minecraft
{
	Renderer::Renderer() : m_VBO(GL_ARRAY_BUFFER)
	{
		// Manage the vertex arrays


		m_DefaultShader.CreateShaderProgramFromFile("Core\\Shaders\\vertex.glsl", "Core\\Shaders\\fragment.glsl");
		m_DefaultShader.CompileShaders();
	}

	void Renderer::RenderChunk(Chunk* chunk, FPSCamera* camera)
	{
		m_DefaultShader.Use();
		
		m_DefaultShader.SetMatrix4("u_Model", glm::mat4(1.0f));
		m_DefaultShader.SetMatrix4("u_View", camera->GetViewProjection());
		m_DefaultShader.SetMatrix4("u_Projection", glm::mat4(1.0f));

		chunk->GetChunkMesh()->p_VAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, chunk->GetChunkMesh()->p_Vertices.size() * 6);
		chunk->GetChunkMesh()->p_VAO.Unbind();
	}
}