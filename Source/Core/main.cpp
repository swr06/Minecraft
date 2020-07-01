#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL Classes/VertexBuffer.h"
#include "OpenGL Classes/VertexArray.h"
#include "OpenGL Classes/IndexBuffer.h"
#include "OpenGL Classes/Shader.h"
#include "OpenGL Classes/Texture.h"
#include "OpenGL Classes/Fps.h"

#include "Application/Application.h"

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
