#include "Shader.h"

namespace GLClasses
{
	static string GetFileName(string path)
	{
		std::filesystem::path pth = std::filesystem::path(path.c_str()).filename();
		return pth.string();
	}

	Shader::~Shader()
	{
		glUseProgram(0);
	}

	void Shader::CompileShaders()
	{
		m_CompiledShader = true;
		auto start = chrono::steady_clock::now();

		GLuint vs;
		GLuint fs;
		GLint successful;
		GLchar GLInfoLog[512];

		vs = glCreateShader(GL_VERTEX_SHADER);
		fs = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vs_char = m_VertexData.c_str();
		const char* fs_char = m_FragmentData.c_str();

		glShaderSource(vs, 1, &vs_char, 0);
		glShaderSource(fs, 1, &fs_char, 0);

		glCompileShader(vs);

		glGetShaderiv(vs, GL_COMPILE_STATUS, &successful);

		if (!successful)
		{
			glGetShaderInfoLog(vs, 512, NULL, GLInfoLog);
			std::cout << "\nCOMPILATION ERROR IN VERTEX SHADER (" << m_VertexPath << ")" << "\n" << GLInfoLog << "\n\n";
		}

		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &successful);

		if (!successful)
		{
			glGetShaderInfoLog(fs, 512, NULL, GLInfoLog);
			std::cout << "\nCOMPILATION ERROR IN FRAGMENT SHADER (" << m_FragmentPath << ")" << "\n" << GLInfoLog << "\n";
		}

		m_Program = glCreateProgram();
		glAttachShader(m_Program, vs);
		glAttachShader(m_Program, fs);
		glLinkProgram(m_Program);

		glGetProgramiv(m_Program, GL_LINK_STATUS, &successful);

		if (!successful)
		{
			glGetProgramInfoLog(m_Program, 512, NULL, GLInfoLog);
			std::cout << "ERROR : SHADER LINKING FAILED : \n" << GLInfoLog << std::endl;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		auto end = chrono::steady_clock::now();
		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
	}

	void Shader::CreateShaderProgramFromFile(const string vertex_pth, const string fragment_pth)
	{
		stringstream v_cont;
		stringstream f_cont;
		string vertex_cont;
		string frag_cont;
		ifstream vertex_file;
		ifstream frag_file;

		vertex_file.exceptions(ifstream::badbit | ifstream::failbit);
		frag_file.exceptions(ifstream::badbit | ifstream::failbit);

		vertex_file.open(vertex_pth, ios::in);
		frag_file.open(fragment_pth, ios::in);
		
		m_VertexPath = vertex_pth;
		m_FragmentPath = fragment_pth;

		if (vertex_file.good() && frag_file.good())
		{
			v_cont << vertex_file.rdbuf();
			f_cont << frag_file.rdbuf();
			m_VertexData = v_cont.str();
			m_FragmentData = f_cont.str();

			vertex_file.close();
			frag_file.close();
		}
	}

	void Shader::CreateShaderProgramFromString(const string& vertex_data, const string& fragment_data)
	{
		m_VertexData = vertex_data;
		m_FragmentData = fragment_data;
		m_VertexPath = "PASSED_VIA_DATA";
		m_FragmentPath = "PASSED_VIA_DATA";
	}

	GLuint Shader::GetProgramID()
	{
		return m_Program;
	}

	void Shader::SetFloat(const GLchar* name, GLfloat value, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetInteger(const GLchar* name, GLint value, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetIntegerArray(const GLchar* name, const GLint* value, GLsizei count, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform2f(GetUniformLocation(name), x, y);
	}

	void Shader::SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform3f(GetUniformLocation(name), x, y, z);
	}

	void Shader::SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform4f(GetUniformLocation(name), x, y, z, w);
	}

	void Shader::SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		}

		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader)
	{
		if (useShader)
		{
			this->Use();
		} 

		//glUniformMatrix4fv(glGetUniformLocation(this->Program, name), 1, GL_FALSE, glm::value_ptr(matrix));
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	GLint Shader::GetUniformLocation(string uniform_name)
	{
		if (Location_map.find(uniform_name) == Location_map.end())
		{
			GLint loc = glGetUniformLocation(this->m_Program, uniform_name.c_str());

			if (loc == -1)
			{
				std::cout << "\nERROR! : UNIFORM NOT FOUND!    |    UNIFORM : " << uniform_name << "  \n\n";
			}

			Location_map[uniform_name] = loc;
		}

		return Location_map[uniform_name];
	}
}