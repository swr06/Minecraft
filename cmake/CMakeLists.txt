cmake_minimum_required(VERSION 3.11)
project(minecraft)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(main
	./main.cpp
	./Noise/FastNoise.cpp
	./Camera.cpp
	./FpsCamera.cpp
	./OpenGL\ Classes/CubeTextureMap.cpp
	./OpenGL\ Classes/IndexBuffer.cpp
	./OpenGL\ Classes/Random.cpp
	./OpenGL\ Classes/Texture.cpp
	./OpenGL\ Classes/stb_image.cpp
	./OpenGL\ Classes/Shader.cpp
	./OpenGL\ Classes/Fps.cpp
	./OpenGL\ Classes/VertexArray.cpp
	./OpenGL\ Classes/VertexBuffer.cpp
	./OpenGL\ Classes/GLDebug.cpp
	./Chunk.cpp
	./CubeRenderer.cpp
	./TextureAtlas.cpp
	./Player/Player.cpp
	./Renderer/Renderer.cpp
	./Application/Application.cpp
	./Application/Events.cpp
	./Block.cpp
	./World/World.cpp
	./World/Skybox.cpp
	./World/WorldGenerator.cpp
	./World/Structures/WorldStructures.cpp
	./BlockTextureManager.cpp
	./Utils/Logger.cpp
	./Utils/Math.cpp
	./Dependencies/imgui/imgui.cpp
	./Dependencies/imgui/imgui_demo.cpp
	./Dependencies/imgui/imgui_draw.cpp
	./Dependencies/imgui/main.cpp
	./Dependencies/imgui/imgui_impl_glfw_gl3.cpp
	./ChunkMesh.cpp)

target_link_libraries(main PRIVATE GLEW::GLEW glfw OpenGL::GL)
target_compile_features(main PRIVATE cxx_std_17)

file(COPY Shaders Resources DESTINATION ${CMAKE_BINARY_DIR}/Core/)
