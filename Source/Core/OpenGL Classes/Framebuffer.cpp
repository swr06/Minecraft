#include "Framebuffer.h"

#ifndef NULL
	#define NULL 0
#endif

namespace GLClasses
{
	Framebuffer::Framebuffer(unsigned int w, unsigned int h)
	{
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			// Create the texture attachment
			GLuint texture;

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		}

		else
		{
			
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
	}
}