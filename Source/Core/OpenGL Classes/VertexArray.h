#pragma once

#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

namespace GLClasses
{
	using namespace std; 

	class VertexArray
	{
	public:

		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();

	private:

		GLuint array_id;
		GLenum type;
	};
}
