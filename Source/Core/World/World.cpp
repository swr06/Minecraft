#include "World.h"

namespace Minecraft
{
	World::World()
	{
		// Generate all the chunks 

		p_Player = new Player;

		Logger::LogToConsole("World Generation Began");

		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				GenerateChunk(&m_WorldChunks[i][j]);
			}
		}

		Logger::LogToConsole("World Generation Ended");
		Logger::LogToConsole("Chunk Mesh construction began");

		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				Timer timer("Construction Begin!");

				m_WorldChunks[i][j].Construct(glm::vec3(i,1,j));
			}
		}

		Logger::LogToConsole("Chunk Mesh construction ended");
	}

	World::~World()
	{

	}

	void World::OnUpdate(GLFWwindow* window)
	{
		p_Player->OnUpdate();

		const float camera_speed = 0.25f;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(Minecraft::MoveDirection::Front, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(Minecraft::MoveDirection::Back, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(Minecraft::MoveDirection::Left, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(Minecraft::MoveDirection::Right, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(Minecraft::MoveDirection::Up, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(Minecraft::MoveDirection::Down, camera_speed);
	}

	void World::RenderWorld()
	{
		const int render_distance = 4;

		std::vector<std::pair<int, int>> chunks;
		
		int player_chunk_x = 0;
		int player_chunk_y = 0;
		int player_chunk_z = 0;

		if (p_Player->p_Position.x < 1)
		{
			player_chunk_x = 0;
		}

		else
		{
			player_chunk_x = floor(p_Player->p_Position.x / ChunkSizeX);
		}

		if (p_Player->p_Position.y < 1)
		{
			player_chunk_y = 0;
		}

		else
		{
			player_chunk_y = floor(p_Player->p_Position.y / ChunkSizeY);
		}

		if (p_Player->p_Position.z < 1)
		{
			player_chunk_z = 0;
		}

		else
		{
			player_chunk_z = floor(p_Player->p_Position.z / ChunkSizeZ);
		}

		/*for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_Renderer.RenderChunk(&m_WorldChunks[i][j], &p_Player->p_Camera);
			}
		}*/

		// Render the player chunk
		m_Renderer.RenderChunk(&m_WorldChunks[player_chunk_x][player_chunk_z], &p_Player->p_Camera);

		if (player_chunk_x < 16)
		{
			m_Renderer.RenderChunk(&m_WorldChunks[player_chunk_x + 1][player_chunk_z], &p_Player->p_Camera);
		}

		if (player_chunk_x > 0)
		{
			m_Renderer.RenderChunk(&m_WorldChunks[player_chunk_x - 1][player_chunk_z], &p_Player->p_Camera);
		}

		if (player_chunk_z < 16)
		{
			m_Renderer.RenderChunk(&m_WorldChunks[player_chunk_x][player_chunk_z + 1], &p_Player->p_Camera);
		}

		if (player_chunk_z > 0)
		{
			m_Renderer.RenderChunk(&m_WorldChunks[player_chunk_x][player_chunk_z - 1], &p_Player->p_Camera);
		}
	}

	void World::OnEvent(EventSystem::Event e)
	{
		if (e.type == EventSystem::EventTypes::MouseMove)
		{
			p_Player->p_Camera.UpdateOnMouseMovement(e.mx, e.my);
		}
	}
}