#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\VertexBuffer.h"
#include "Core\OpenGL Classes\VertexArray.h"
#include "Core\OpenGL Classes\IndexBuffer.h"
#include "Core\OpenGL Classes\Shader.h"
#include "Core\OpenGL Classes\Texture.h"
#include "Core\OpenGL Classes\Fps.h"

#include "Core\Application\Application.h"

#include <iostream>

int main()
{
    Minecraft::Application minecraft;

    while (!glfwWindowShouldClose(minecraft.m_Window))
    {
        minecraft.OnUpdate();
    }

    return 0;
}

