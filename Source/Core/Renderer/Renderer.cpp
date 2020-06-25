#include "Renderer.h"

namespace Minecraft
{
	Renderer::Renderer() : m_VBO(GL_ARRAY_BUFFER)
	{
		// Manage the vertex arrays

		m_VAO.Bind();
		m_VBO.Bind();
		m_VBO.BufferData(1000000 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		m_VBO.VertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
		m_VAO.Unbind();

		m_DefaultShader.CreateShaderProgramFromFile("Core\\Shaders\\vertex.glsl", "Core\\Shaders\\fragment.glsl");
		m_DefaultShader.CompileShaders();
	}

	void Renderer::RenderChunk(Chunk* chunk, FPSCamera* camera)
	{
		m_DefaultShader.Use();
		
		m_DefaultShader.SetMatrix4("u_Model", glm::mat4(1.0f));
		m_DefaultShader.SetMatrix4("u_View", camera->GetViewProjection());
		m_DefaultShader.SetMatrix4("u_Projection", glm::mat4(1.0f));

		m_VAO.Bind();
		m_VBO.BufferSubData(0, chunk->GetChunkMesh()->p_Vertices.size() * sizeof(Vertex), 
			&chunk->GetChunkMesh()->p_Vertices.front());

		glDrawArrays(GL_TRIANGLES, 0, chunk->GetChunkMesh()->p_Vertices.size() * 6);
		m_VAO.Unbind();
	}
}