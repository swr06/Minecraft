#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <filesystem>

namespace GLClasses
{
	using namespace std;
	class Shader
	{
	public:

		Shader(string vertex_shader, const string fragment_shader, bool pass_by_path)
		{
			if (pass_by_path)
			{
				CreateShaderProgramFromFile(vertex_shader, fragment_shader);
			}

			else
			{
				CreateShaderProgramFromString(vertex_shader, fragment_shader);
			}
		}

		Shader() {}; 

		~Shader();

		void CompileShaders();
		void CreateShaderProgramFromFile(const string vertex_pth, const string fragment_pth);
		void CreateShaderProgramFromString(const string& vertex_data, const string& fragment_data);
		GLuint GetProgramID();
		
		inline void Use()
		{
			if (m_CompiledShader == false)
			{
				this->CompileShaders();
			}

			glUseProgram(this->m_Program);
		}

		void SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = GL_FALSE);
		void SetInteger(const GLchar* name, GLint value, GLboolean useShader = GL_FALSE);
		void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = GL_FALSE);
		void SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = GL_FALSE);
		void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = GL_FALSE);
		void SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = GL_FALSE);
		void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = GL_FALSE);
		void SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = GL_FALSE);
		void SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = GL_FALSE);
		void SetIntegerArray(const GLchar* name, const GLint* value, GLsizei count, GLboolean useShader = GL_FALSE);

	 private:

		unordered_map<string, GLint> Location_map; // To avoid unnecessary calls to glGetUniformLocation()
		GLuint m_Program;
		bool m_CompiledShader = false;

		GLint GetUniformLocation(string uniform_name);
		string m_VertexData;
		string m_VertexPath;
		string m_FragmentData;
		string m_FragmentPath;
	};
}
