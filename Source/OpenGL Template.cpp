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

#include "Core\CubeRenderer.h"
#include "Core\Camera.h"

#include <iostream>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); 
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace GLClasses;

void CameraMove(GLFWwindow* window, double xpos, double ypos);

Minecraft::Camera camera(45.0f, SCR_WIDTH / SCR_HEIGHT, 0.1, 100.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
    glewInit();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, CameraMove);
    glEnable(GL_DEPTH_TEST);

    Texture texture("Core\\Resources\\grass_block.png");
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  10.0f, -12.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float) SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
    Minecraft::CubeRenderer cb;

    float angle = 0;
    const int block_count = 16;

    glm::vec3 cube_position = glm::vec3(0.0f, 0.0f, -4.0f);

    while (!glfwWindowShouldClose(window))
    {
        angle += 3;
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        cb.RenderCube(cube_position, &texture, angle, camera.GetViewProjection());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    const float camera_speed = 0.1f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ChangePosition(camera.GetFront() * camera_speed);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ChangePosition(-(camera.GetFront() * camera_speed));

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ChangePosition(-(camera.GetRight() * camera_speed));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ChangePosition(camera.GetRight() * camera_speed);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ChangePosition(camera.GetUp() * camera_speed);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ChangePosition(-(camera.GetUp() * camera_speed));
}

void CameraMove(GLFWwindow* window, double xpos, double ypos)
{
    static bool first_move = false;

    const float sensitivity = 0.2;
    static float prev_mx = 0.0f;
    static float prev_my = 0.0f;
    static float yaw = 0.0f;
    static float pitch = 0.0f;

    ypos = -ypos;

    float x_diff = xpos - prev_mx;
    float y_diff = ypos - prev_my;

    if (first_move == false)
    {
        first_move = true;
        prev_mx = xpos;
        prev_my = ypos;
    }


    // Apply the sensitivity 
    x_diff = x_diff * sensitivity;
    y_diff = y_diff * sensitivity;

    prev_mx = xpos;
    prev_my = ypos;

    yaw = yaw + x_diff;
    pitch = pitch + y_diff;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 front; 
    
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch)); 
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    camera.SetFront(front);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    camera.SetAspect(width / height);
}
