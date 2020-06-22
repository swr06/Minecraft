#pragma once

#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

namespace GLClasses
{
	class IndexBuffer
	{
	public:

		IndexBuffer();
		~IndexBuffer();

		void BufferData(GLsizeiptr size, void* data, GLenum usage);
		void Bind();
		void Unbind();


	private:

		GLuint buffer_id;
		GLenum type;
	};

}