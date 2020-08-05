#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLClass includes
#include "../OpenGL Classes/Texture.h"
#include "../OpenGL Classes/VertexArray.h"
#include "../OpenGL Classes/VertexBuffer.h"
#include "../OpenGL Classes/IndexBuffer.h"
#include "../OpenGL Classes/Shader.h"
#include "../OpenGL Classes/GLDebug.h"

#include "../Utils/Defs.h"

namespace Minecraft
{
	namespace Clouds
	{
		void Init();
		void RenderClouds(double ts, const glm::vec2& render_distance);
	}
}