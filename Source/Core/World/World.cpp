#include "World.h"

namespace Minecraft
{
	World::World() : m_Camera2D(0.0f, 800.0f, 0.0f, 600.0f)
	{
		// Generate all the chunks 

		p_Player = new Player;

		// Set the players position
		p_Player->p_Camera.SetPosition(glm::vec3(2 * 16, 10 * 16, 2 * 16));
		p_Player->p_Position = glm::vec3(2 * 16, 2 * 16, 2 * 16);

		Logger::LogToConsole("World Generation Began");

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Timer timer("First Chunk generation");

				m_WorldChunks[i][j].p_Position = glm::vec3(i, 1, j);
				GenerateChunk(&m_WorldChunks[i][j]);
			}
		}

		Logger::LogToConsole("World Generation Ended");
		Logger::LogToConsole("Chunk Mesh construction began");

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Timer timer("First Chunks Construction!");

				m_WorldChunks[i][j].Construct(glm::vec3(i,1,j));
			}
		}

		Logger::LogToConsole("Chunk Mesh construction ended");

		m_CrosshairTexture.CreateTexture("Resources/crosshair.png");
	}

	World::~World()
	{

	}

	void World::OnUpdate(GLFWwindow* window)
	{
		p_Player->OnUpdate();

		const float camera_speed = 0.35f;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Front, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Back, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Left, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Right, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Up, camera_speed);

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			p_Player->p_Camera.MoveCamera(MoveDirection::Down, camera_speed);

		//////

		// Get the current player's chunk
		int player_chunk_x = 0;
		int player_chunk_y = 0;
		int player_chunk_z = 0;

		player_chunk_x = floor(p_Player->p_Position.x / ChunkSizeX);
		player_chunk_y = floor(p_Player->p_Position.y / ChunkSizeY);
		player_chunk_z = floor(p_Player->p_Position.z / ChunkSizeZ);

		// Cast a ray from the player's current position

		glm::vec3 block_pos;
		glm::vec3 ray_end;

		for (RayCast ray(&p_Player->p_Camera); ray.GetLength() < 6; ray.StepRay(0.05))
		{
			ray_end = ray.GetEnd();

			//std::cout << std::endl << "Block Location | X : " << block_pos.x << " | Y : " << block_pos.y << " | Z : " << block_pos.z;

			Block* block = GetWorldBlock(ray_end);

			if (block != nullptr)
			{
				block_pos = block->p_Position;

				if (block->p_BlockType != BlockType::Air)
				{
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
					{
						std::cout << std::endl << "Block Location | X : " << block_pos.x << " | Y : " << block_pos.y << " | Z : " << block_pos.z;
					}
				}
			}
		}
	}

	void World::RenderWorld()
	{
		int player_chunk_x = 0;
		int player_chunk_y = 0;
		int player_chunk_z = 0;

		m_Skybox.RenderSkybox(&p_Player->p_Camera);

		player_chunk_x = floor(p_Player->p_Position.x / ChunkSizeX);
		player_chunk_y = floor(p_Player->p_Position.y / ChunkSizeY);
		player_chunk_z = floor(p_Player->p_Position.z / ChunkSizeZ);

		int render_distance_x = 2, render_distance_z = 2;

		for (int i = player_chunk_x - render_distance_x; i < player_chunk_x + render_distance_x; i++)
		{
			for (int j = player_chunk_z - render_distance_z; j < player_chunk_z + render_distance_z; j++)
			{
				RenderChunkFromMap(i, j);
			}
		}

		// Todo : Render crosshair and other 2D elements
	}

	void World::OnEvent(EventSystem::Event e)
	{
		p_Player->OnEvent(e);

		if (e.type == EventSystem::EventTypes::MousePress)
		{
	
		}
	}

	Block* World::GetWorldBlock(const glm::vec3& block_loc)
	{
		int block_chunk_x = static_cast<int>(floor(block_loc.x / ChunkSizeX));
		int block_chunk_z = static_cast<int>(floor(block_loc.z / ChunkSizeZ));
		int bx = (int)block_loc.x % ChunkSizeX;
		int by = (int)block_loc.y % ChunkSizeY;
		int bz = (int)block_loc.z % ChunkSizeZ;

		Chunk* chunk = GetChunkFromMap(block_chunk_x, block_chunk_z);

		if (bx >= 0 && by >= 0 && bz >= 0 &&
			bx < ChunkSizeX && bz < ChunkSizeY && bz < ChunkSizeZ)
		{
			return &chunk->m_ChunkContents->at(bx).at(by).at(bz);
		}

		else
		{
			return nullptr;
		}
	}

	void World::RenderChunkFromMap(int cx, int cz)
	{
		m_Renderer.RenderChunk(GetChunkFromMap(cx, cz), &p_Player->p_Camera);
		return;
	}

	Chunk* World::GetChunkFromMap(int cx, int cz)
	{
		std::stringstream str;

		str << "Chunk Building ! X : " << cx << " | Z : " << cz;

		std::map<int, std::map<int, Chunk>>::iterator chunk_exists_x = m_WorldChunks.find(cx);

		if (chunk_exists_x != m_WorldChunks.end())
		{
			std::map<int, Chunk>::iterator chunk_exists_z = m_WorldChunks.at(cx).find(cz);

			if (chunk_exists_z != m_WorldChunks.at(cx).end())
			{
				// The chunk exists. Return the address of the retrieved chunk
				return &m_WorldChunks.at(cx).at(cz);
			}

			else
			{
				Timer timer(str.str());

				m_WorldChunks[cx][cz].p_Position = glm::vec3(cx, 1, cz);
				GenerateChunk(&m_WorldChunks[cx][cz]);
				m_WorldChunks[cx][cz].Construct(glm::vec3(cx, 1, cz));
			}
		}

		else
		{
			Timer timer(str.str());

			m_WorldChunks[cx][cz].p_Position = glm::vec3(cx, 1, cz);
			GenerateChunk(&m_WorldChunks[cx][cz]);
			m_WorldChunks[cx][cz].Construct(glm::vec3(cx, 1, cz));
		}

		return &m_WorldChunks.at(cx).at(cz);
	}
}