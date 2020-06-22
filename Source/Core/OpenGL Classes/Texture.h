#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <string>
#include <array>
#include <chrono>

namespace GLClasses
{
	using namespace std;

	class Texture
	{
	public:

		Texture(string path, GLint internalformat = GL_RGBA, GLenum type = GL_TEXTURE_2D,
			GLenum min_filter = GL_NEAREST, GLenum mag_filter = GL_NEAREST,
			GLenum texwrap_s = GL_REPEAT, GLenum texwrap_t = GL_REPEAT,
			array<GLfloat, 8> TextureCoords = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
		, bool clean_up = true);

		Texture()
		{
			m_Texture = new GLuint;
			m_TextureCoords = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
			m_clean_up = true;
			m_width = 0;
			m_height = 0;
			m_Texture = 0;
			m_type = GL_TEXTURE_2D;
			m_intformat = GL_RGBA;
			m_BPP = 0;
			m_delete_texture = true;
		}

		~Texture()
		{
			if (this->m_delete_texture == 1 && this->m_Texture != nullptr)
			{
				glDeleteTextures(1, m_Texture);
				m_Texture = nullptr;
			}

			else
			{
				// Don't delete
			}
		}

		void CreateTexture(string path, GLint internalformat = GL_RGBA, GLenum type = GL_TEXTURE_2D,
			GLenum min_filter = GL_NEAREST, GLenum mag_filter = GL_NEAREST,
			GLenum texwrap_s = GL_REPEAT, GLenum texwrap_t = GL_REPEAT, array<GLfloat, 8> TextureCoords = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, bool clean_up = true);

		void IntCreateTexture(GLuint id, array<GLfloat, 8> tex_coords, int w, int h, bool delete_texture);

		inline int GetWidth() const
		{
			return m_width;
		}

		inline int GetHeight() const
		{
			return m_height;
		}

		inline void Bind(int slot = 0) const
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(this->m_type, *(this->m_Texture));
		}

		inline void Unbind() const
		{
			glBindTexture(m_type, 0);
		}

		inline GLuint GetTextureID() const
		{
			return *m_Texture;
		};

		inline string GetTexturePath() const
		{
			return m_path;
		}

		array<GLfloat, 8> GetTextureCoords() const
		{
			return m_TextureCoords;
		}

		int m_delete_texture;

	private:

		bool m_clean_up;

		int m_width;
		int m_height;
		int m_BPP;
		GLint m_intformat;
		GLuint* m_Texture;
		GLenum m_type;
		string m_path;
		array<GLfloat, 8> m_TextureCoords;
	};
}