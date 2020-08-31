#include "ParticleSystem.h"

Minecraft::ParticleSystem::ParticleRenderer::ParticleRenderer()
{
}

void Minecraft::ParticleSystem::ParticleRenderer::RenderParticle(const Particle& particle)
{
	GLuint indices[6] = { 0,1,2,2,3,0 };
	GLfloat vertices[8] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };

	GLClasses::VertexBuffer VBO(GL_ARRAY_BUFFER);
	GLClasses::VertexArray VAO;
	GLClasses::IndexBuffer IBO;
	VAO.Bind();
	VBO.Bind();
	IBO.Bind();
	VBO.BufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	IBO.BufferData(sizeof(indices), indices, GL_STATIC_DRAW);	
	VAO.Unbind();

	VAO.Bind();
	DebugGLFunction(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	VAO.Unbind();
	
}
