/*
-- Project -- 
Title : Tiny Minecraft Clone
Version : V0.03

-- Dev -- 
Name : Samuel Wesley Rasquinha
Email : samuelrasquinha@gmail.com
Github : @swr06
*/

// If you like this project, please star it on github <3

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/OpenGL Classes/VertexBuffer.h"
#include "Core/OpenGL Classes/VertexArray.h"
#include "Core/OpenGL Classes/IndexBuffer.h"
#include "Core/OpenGL Classes/Shader.h"
#include "Core/OpenGL Classes/Texture.h"
#include "Core/OpenGL Classes/Fps.h"

#include "Core/Application/Application.h"

#include <iostream>

// Uses the already declared application variable
extern Minecraft::Application Minecraft::MinecraftApplication;

// Forward declarations
namespace Minecraft
{
    /*
    This function is used by the chunk mesher to get the data of the neighbouring chunks
    */

    ChunkDataTypePtr _GetChunkDataForMeshing (int cx, int cz)
    {
        if (MinecraftApplication.GetWorld() && MinecraftApplication.GetWorld()->ChunkExistsInMap(cx, cz))
        {
            Chunk* chunk = MinecraftApplication.GetWorld()->RetrieveChunkFromMap(cx, cz);
            return &chunk->p_ChunkContents;
        }

        return nullptr;
    }

    ChunkLightDataTypePtr _GetChunkLightDataForMeshing(int cx, int cz)
    {
        if (MinecraftApplication.GetWorld() && MinecraftApplication.GetWorld()->ChunkExistsInMap(cx, cz))
        {
            Chunk* chunk = MinecraftApplication.GetWorld()->RetrieveChunkFromMap(cx, cz);
            return &chunk->p_ChunkLightInformation;
        }

        return nullptr;
    }

    Block* GetWorldBlock(const glm::vec3& block_pos)
    {
        std::pair<Block*, Chunk*> block = MinecraftApplication.GetWorld()->GetBlockFromPosition(block_pos);
        return block.first;
    }
}

int main()
{
    // The main game loop

    while (!glfwWindowShouldClose(Minecraft::MinecraftApplication.m_Window))
    {
        Minecraft::MinecraftApplication.OnUpdate();
    }

    return 0;
}

