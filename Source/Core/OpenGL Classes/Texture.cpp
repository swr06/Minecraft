#include "Texture.h"

namespace GLClasses
{
	Texture::Texture(string path, GLenum type, GLenum min_filter, GLenum mag_filter, GLenum texwrap_s, GLenum texwrap_t, array<GLfloat, 8> TextureCoords, bool clean_up)
	{
		stbi_set_flip_vertically_on_load(true);

		m_Texture = new GLuint;
		this->CreateTexture(path, type, min_filter, mag_filter, texwrap_s, texwrap_t, TextureCoords, clean_up);	
	}

	void Texture::CreateTexture(string path, GLenum type, GLenum min_filter, GLenum mag_filter, GLenum texwrap_s, GLenum texwrap_t, array<GLfloat, 8> TextureCoords, bool clean_up)
	{
		GLenum internalformat = GL_RGBA;
		stbi_set_flip_vertically_on_load(true); 
		
		m_delete_texture = true;
		m_Texture = new GLuint;
		m_TextureCoords = TextureCoords; 
		m_clean_up = clean_up;
		m_type = type;
		m_path = path;

		glGenTextures(1, m_Texture);
		glBindTexture(type, *m_Texture);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, texwrap_s);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, texwrap_t);

		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag_filter);

		unsigned char* image = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

		if (m_BPP == 1)
		{
			internalformat = GL_RED;
		}

		else if (m_BPP == 3)
		{
			internalformat = GL_RGB;
		}

		else if (m_BPP == 4)
		{
			internalformat = GL_RGBA;
		}

		glTexImage2D(type, 0, internalformat, m_width, m_height, 0, internalformat, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(type);

		if (image && clean_up)
		{
			stbi_image_free(image);
		}
	}

	void Texture::IntCreateTexture(GLuint id, array<GLfloat, 8> tex_coords, int w, int h, bool delete_texture) // To use for Texture Atlases
	{
		m_Texture = new GLuint;
		*m_Texture = id;
		m_TextureCoords = tex_coords;
		m_width = w;
		m_height = h;
		m_delete_texture = delete_texture;
	}

	ExtractedImageData ExtractTextureData(const std::string& path)
	{
		ExtractedImageData return_val;

		return_val.image_data = stbi_load(path.c_str(), &return_val.width, &return_val.height, &return_val.channels, 4);
		return return_val;
	}
}