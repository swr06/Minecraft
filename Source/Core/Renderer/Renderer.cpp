#include "Renderer.h"

namespace Minecraft
{
	Renderer::Renderer() : m_VBO(GL_ARRAY_BUFFER)
	{
		// Manage the vertex arrays

		m_DefaultShader.CreateShaderProgramFromFile("Core\\Shaders\\vertex.glsl", "Core\\Shaders\\fragment.glsl");
		m_DefaultShader.CompileShaders();
		m_BlockAtlas.CreateTexture("Core\\Resources\\block_atlas.png");
	}

	void Renderer::RenderChunk(Chunk* chunk, FPSCamera* camera)
	{
		m_DefaultShader.Use();
		m_BlockAtlas.Bind(0);
		m_DefaultShader.SetInteger("u_Texture", 0, 0);
		m_DefaultShader.SetMatrix4("u_Model", glm::mat4(1.0f));
		m_DefaultShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());

		chunk->GetChunkMesh()->p_VAO.Bind();
		DebugGLFunction(glDrawArrays(GL_TRIANGLES, 0, chunk->GetChunkMesh()->p_Vertices.size() * 6));
		chunk->GetChunkMesh()->p_VAO.Unbind();
	}
}