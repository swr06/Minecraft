#pragma once

#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

namespace GLClasses
{
	using namespace std;

	class VertexBuffer
	{
	public:

		VertexBuffer(GLenum type);
		~VertexBuffer();

		void BufferData(GLsizeiptr size, void* data, GLenum usage);
		void Bind();
		void Unbind();
		void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized,
			GLsizei stride, const GLvoid* pointer);

	private:

		GLuint buffer_id;
		GLenum type;
	};
}