#include "IndexBuffer.h"

namespace GLClasses
{
	IndexBuffer::IndexBuffer()
	{
		this->buffer_id = 0;
		this->type = GL_ELEMENT_ARRAY_BUFFER;
		glGenBuffers(1, &(this->buffer_id));
		this->Bind();
	}

	void IndexBuffer::BufferData(GLsizeiptr size, void* data, GLenum usage)
	{
		this->Bind();
		glBufferData(this->type, size, data, usage);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &(this->buffer_id));
		this->Unbind();
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(this->type, buffer_id);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(this->type, 0);
		glBindVertexArray(0);
	}

}