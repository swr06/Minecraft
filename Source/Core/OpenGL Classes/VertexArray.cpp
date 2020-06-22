#include "VertexArray.h"

namespace GLClasses
{
	VertexArray::VertexArray()
	{
		this->array_id = 0;
		glGenVertexArrays(1, &(this->array_id));
		this->Bind();
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &(this->array_id));
		this->Unbind();
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(this->array_id);
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
}