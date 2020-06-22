#include "GLDebug.h"

GLenum CheckOpenGLError(const char* File, int Line)
{
	GLenum ERR_CODE;
	std::string PRINT_ERROR;

	ERR_CODE = glGetError();

	if (ERR_CODE != GL_NO_ERROR)
	{
		switch (ERR_CODE)
		{
			case GL_INVALID_ENUM:
			{
				PRINT_ERROR = "INVALID_ENUM";
				break;
			}

			case GL_INVALID_VALUE:
			{
				PRINT_ERROR = "INVALID_VALUE";
				break;
			}

			case GL_INVALID_OPERATION:
			{
				PRINT_ERROR = "INVALID_OPERATION";
				break;
			}

			case GL_STACK_OVERFLOW:
			{
				PRINT_ERROR = "STACK_OVERFLOW";
				break;
			}

			case GL_STACK_UNDERFLOW:
			{
				PRINT_ERROR = "STACK_UNDERFLOW";
				break;
			}

			case GL_OUT_OF_MEMORY:
			{
				PRINT_ERROR = "OUT_OF_MEMORY";
				break;
			}

			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				PRINT_ERROR = "INVALID_FRAMEBUFFER_OPERATION";
				break;
			}
		}

		std::cout << "\nOpenGL Error in Line " << Line << " and in File : " << (File) << "\n\tError : " << PRINT_ERROR << "\n";
	}

	return ERR_CODE;
}