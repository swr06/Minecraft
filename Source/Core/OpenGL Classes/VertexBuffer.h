#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <string>

#include "GLDebug.h"

namespace GLClasses
{
	using namespace std;

	class VertexBuffer
	{
	public:

		VertexBuffer(GLenum type);
		~VertexBuffer();

		void BufferData(GLsizeiptr size, void* data, GLenum usage);
		void BufferSubData(GLintptr offset, GLsizeiptr size, void* data);
		void Bind();
		void Unbind();
		void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized,
			GLsizei stride, const GLvoid* pointer);

		void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);

	private:

		GLuint buffer_id;
		GLenum type;
	};
}