#include "World.h"

namespace Minecraft
{
	static void PrintVec3(const glm::vec3& val)
	{
		std::cout << std::endl << "X : " << val.x << " Y : " << val.y << " Z : " << val.z;
		return;
	}

	World::World() : m_Camera2D(0.0f, 800.0f, 0.0f, 600.0f)
	{
		// Generate all the chunks 

		p_Player = new Player;

		// Set the players position
		p_Player->p_Camera.SetPosition(glm::vec3(0, (ChunkSizeY - MaxStructureY) - 2, 0));

		Logger::LogToConsole("World Generation Began");

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Timer timer("First Chunk generation");

				m_WorldChunks[std::pair<int, int>(i, j)].p_Position = glm::vec3(i, 1, j);
				GenerateChunk(&m_WorldChunks[std::pair<int, int>(i, j)]);
				m_ChunkCount++;
			}
		}

		Logger::LogToConsole("World Generation Ended");
		Logger::LogToConsole("Chunk Mesh construction began");

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Timer timer("First Chunks Construction!");

				m_WorldChunks[std::pair<int, int>(i, j)].Construct();
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

		float block_dist = 16.0f;
		static float block_delta = glfwGetTime();

		//// Get position relative to camera's direction
		//std::vector<glm::vec3> traversed_voxels;
		//FastVoxelTraversal(p_Player->p_Camera.GetPosition(),
		//	p_Player->p_Camera.GetPosition() + p_Player->p_Camera.GetFront() * block_dist, traversed_voxels);

		//for (int i = 0; i < traversed_voxels.size(); i++)
		//{
		//	// Check if the traversed voxel is valid
		//	if (traversed_voxels[i].y < ChunkSizeY && traversed_voxels[i].y >= 0)
		//	{
		//		std::pair<Block*, Chunk*> block = GetWorldBlockFromPosition(traversed_voxels[i]);

		//		if (block.first->p_BlockType != BlockType::Air && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		//		{
		//			block.second->SetBlock(BlockType::Dirt, block.first->p_Position);
		//			block.second->Construct();

		//			break;
		//		}

		//		else if (block.first->p_BlockType != BlockType::Air &&
		//			glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		//		{
		//			block.second->SetBlock(BlockType::Air, block.first->p_Position);
		//			block.second->Construct();

		//			break;
		//		}
		//	}
		//}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			RayCast(true);
		}

		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			RayCast(false);
		}

	}

	void World::RenderWorld()
	{
		int player_chunk_x = 0;
		int player_chunk_y = 0;
		int player_chunk_z = 0;

		m_Skybox.RenderSkybox(&p_Player->p_Camera);

		player_chunk_x = floor(p_Player->p_Position.x / ChunkSizeX);
		player_chunk_z = floor(p_Player->p_Position.z / ChunkSizeZ);

		int render_distance_x = 2, render_distance_z = 2;
		int chunks_rendered = 0;

		// Render chunks according to render distance
		for (int i = player_chunk_x - render_distance_x; i < player_chunk_x + render_distance_x; i++)
		{
			for (int j = player_chunk_z - render_distance_z; j < player_chunk_z + render_distance_z; j++)
			{
				chunks_rendered++;
				RenderChunkFromMap(i, j);
			}
		}

		m_Renderer2D.RenderQuad(glm::vec3(400.0f - (m_CrosshairTexture.GetWidth() / 2), 300.0f - (m_CrosshairTexture.GetHeight() / 2), 1.0f), &m_CrosshairTexture, &m_Camera2D);

	}

	void World::OnEvent(EventSystem::Event e)
	{
		p_Player->OnEvent(e);

		if (e.type == EventSystem::EventTypes::MousePress)
		{

		}
	}

	std::pair<Block*, Chunk*> World::GetWorldBlockFromPosition(const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / ChunkSizeX));
		int block_chunk_z = static_cast<int>(floor(pos.z / ChunkSizeZ));
		int bx = pos.x - (block_chunk_x * ChunkSizeX);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * ChunkSizeZ);

		Chunk* chunk = GetChunkFromMap(block_chunk_x, block_chunk_z);

		return { &chunk->m_ChunkContents->at(bx).at(by).at(bz), chunk };
	}

	void World::SetWorldBlockFromPosition(BlockType type, const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / ChunkSizeX));
		int block_chunk_z = static_cast<int>(floor(pos.z / ChunkSizeZ));
		int bx = pos.x - (block_chunk_x * ChunkSizeX);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * ChunkSizeZ);

		GetChunkFromMap(block_chunk_x, block_chunk_z)->SetBlock(type, glm::vec3(bx, by, bz));
	}

	std::pair<Block*, Chunk*> World::GetWorldBlock(const glm::vec3& block_loc)
	{
		int block_chunk_x = static_cast<int>(floor(block_loc.x / ChunkSizeX));
		int block_chunk_z = static_cast<int>(floor(block_loc.z / ChunkSizeZ));

		Chunk* chunk = GetChunkFromMap(block_chunk_x, block_chunk_z);
		return { &chunk->m_ChunkContents->at(block_loc.x).at(block_loc.y).at(block_loc.z), chunk };
	}

	BlockType World::GetWorldBlockTypeFromPosition(const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / ChunkSizeX));
		int block_chunk_z = static_cast<int>(floor(pos.z / ChunkSizeZ));
		int bx = pos.x - (block_chunk_x * ChunkSizeX);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * ChunkSizeZ);

		return GetChunkFromMap(block_chunk_x, block_chunk_z)->m_ChunkContents->at(bx).at(by).at(bz).p_BlockType;
	}

	void World::RenderChunkFromMap(int cx, int cz)
	{
		m_Renderer.RenderChunk(GetChunkFromMap(cx, cz), &p_Player->p_Camera);
		return;
	}

	void World::UnloadFarChunks()
	{
		// TODO : Unload chunks into a binary file
		return;
	}

	void World::RayCast(bool place)
	{
		glm::vec3 position = p_Player->p_Position;
		const glm::vec3& direction = p_Player->p_Camera.GetFront();
		int max = 50; // block reach

		glm::ivec3 blockPos;
		glm::vec3 sign;

		for (int i = 0; i < 3; ++i)
			sign[i] = direction[i] > 0;

		for (int i = 0; i < max; ++i)
		{
			glm::vec3 tvec = (floor(position + sign) - position) / direction;
			float t = std::min(tvec.x, std::min(tvec.y, tvec.z));

			position += direction * (t + 0.001f);

			if (position.y >= 0 && position.y < ChunkSizeY)
			{ 
				std::pair<Block*, Chunk*> ray_hitblock = GetWorldBlockFromPosition(glm::vec3(
					floor(position.x),
					floor(position.y),
					floor(position.z)
				));

				Block* ray_block = ray_hitblock.first;
				Chunk* ray_chunk = ray_hitblock.second;

				if (ray_block != nullptr && ray_block->p_BlockType != BlockType::Air)
				{
					glm::vec3 normal;

					for (int i = 0; i < 3; ++i)
					{
						normal[i] = (t == tvec[i]);

						if (sign[i])
						{
							normal[i] = -normal[i];
						}
					}

					if (place)
					{
						position = position + normal;
					}

					std::pair<Block*, Chunk*> edit_block;

					edit_block = GetWorldBlockFromPosition(glm::vec3(position.x, position.y, position.z));

					if (place)
					{
						edit_block.first->p_BlockType = BlockType::Dirt;
					}

					else
					{
						edit_block.first->p_BlockType = BlockType::Air;
					}

					edit_block.second->Construct();

					return;
				}
			}
		}
	}

	Chunk* World::GetChunkFromMap(int cx, int cz)
	{
		std::stringstream str;

		str << "Chunk Building ! X : " << cx << " | Z : " << cz;

		std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));

		if (chunk_exists == m_WorldChunks.end())
		{
			Timer timer(str.str());

			m_WorldChunks[std::pair<int, int>(cx, cz)].p_Position = glm::vec3(cx, 1, cz);
			GenerateChunk(&m_WorldChunks[std::pair<int, int>(cx, cz)]);
			m_WorldChunks[std::pair<int, int>(cx, cz)].Construct();
			m_ChunkCount++;
		}

		return &m_WorldChunks.at(std::pair<int, int>(cx, cz));
	}
}