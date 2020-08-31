set(Minecraft_source
    Core/Application/Application.cpp
    Core/Application/Application.h
    Core/Application/Events.cpp
    Core/Application/Events.h
    Core/Application/EventStructs.h
    Core/Application/PlatformSpecific.cpp
    Core/Application/PlatformSpecific.h
    Core/Audio/alut.h
    Core/Audio/Audio.cpp
    Core/Audio/Audio.h
    Core/Audio/OpenALFunctions.cpp
    Core/Audio/OpenALFunctions.h
    Core/Clouds/Cloud.cpp
    Core/Clouds/Cloud.h
    Core/File\ Handling/ChunkFileHandler.cpp
    Core/File\ Handling/ChunkFileHandler.h
    Core/File\ Handling/WorldFileHandler.cpp
    Core/File\ Handling/WorldFileHandler.h
    Core/Gameplay/InventorySystem.cpp
    Core/Gameplay/InventorySystem.h
    Core/Lighting/Lighting.cpp
    Core/Lighting/Lighting.h
    Core/Maths/Frustum.cpp
    Core/Maths/Frustum.h
    Core/Models/Model.cpp
    Core/Models/Model.h
    Core/Noise/FastNoise.cpp
    Core/Noise/FastNoise.h
    Core/OpenGL\ Classes/CubeTextureMap.cpp
    Core/OpenGL\ Classes/CubeTextureMap.h
    Core/OpenGL\ Classes/Fps.cpp
    Core/OpenGL\ Classes/Fps.h
    Core/OpenGL\ Classes/GLDebug.cpp
    Core/OpenGL\ Classes/GLDebug.h
    Core/OpenGL\ Classes/IndexBuffer.cpp
    Core/OpenGL\ Classes/IndexBuffer.h
    Core/OpenGL\ Classes/Random.cpp
    Core/OpenGL\ Classes/Random.h
    Core/OpenGL\ Classes/Shader.cpp
    Core/OpenGL\ Classes/Shader.h
    Core/OpenGL\ Classes/stb_image.cpp
    Core/OpenGL\ Classes/stb_image.h
    Core/OpenGL\ Classes/Texture.cpp
    Core/OpenGL\ Classes/Texture.h
    Core/OpenGL\ Classes/VertexArray.cpp
    Core/OpenGL\ Classes/VertexArray.h
    Core/OpenGL\ Classes/VertexBuffer.cpp
    Core/OpenGL\ Classes/VertexBuffer.h
    Core/Player/Player.cpp
    Core/Player/Player.h
    Core/Renderer/Renderer.cpp
    Core/Renderer/Renderer.h
    Core/Renderer/Renderer2D.cpp
    Core/Renderer/Renderer2D.h
    Core/UI/GUI.cpp
    Core/UI/GUI.h
    Core/Utils/Defs.h
    Core/Utils/Enums.h
    Core/Utils/Logger.cpp
    Core/Utils/Logger.h
    Core/Utils/Random.h
    Core/Utils/Ray.h
    Core/Utils/Raycast.cpp
    Core/Utils/Raycast.h
    Core/Utils/Timer.h
    Core/Utils/Vertex.h
    Core/World/Structures/WorldStructures.cpp
    Core/World/Structures/WorldStructures.h
    Core/World/Skybox.cpp
    Core/World/Skybox.h
    Core/World/World.cpp
    Core/World/World.h
    Core/World/WorldGenerator.cpp
    Core/World/WorldGenerator.h
    Core/Block.cpp
    Core/Block.h
    Core/BlockDatabase.cpp
    Core/BlockDatabase.h
    Core/Camera.cpp
    Core/Camera.h
    Core/Chunk.cpp
    Core/Chunk.h
    Core/ChunkMesh.cpp
    Core/ChunkMesh.h
    Core/CubeRenderer.cpp
    Core/CubeRenderer.h
    Core/FpsCamera.cpp
    Core/FpsCamera.h
    Core/OrthographicCamera.cpp
    Core/OrthographicCamera.h
    Core/TextureAtlas.cpp
    Core/TextureAtlas.h
    main.cpp
)

add_executable(Minecraft ${Minecraft_source})

target_link_libraries(Minecraft PRIVATE 
    glfw 
    glew::glew
    glm
    imgui::imgui
)

target_compile_features(Minecraft PRIVATE cxx_std_17)

create_target_launcher(Minecraft
        RUNTIME_LIBRARY_DIRS "${CMAKE_BINARY_DIR}/Core"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Core"
    )
