#include "World.h"

namespace Minecraft
{
	static void PrintVec3(const glm::vec3& val)
	{
		std::cout << std::endl << "X : " << val.x << " Y : " << val.y << " Z : " << val.z;
		return;
	}

	World::World() : m_Camera2D(0.0f, (float)DEFAULT_WINDOW_X, 0.0f, (float)DEFAULT_WINDOW_Y)
	{
		// Generate all the chunks 

		p_Player = new Player;

		// Set the players position
		p_Player->p_Camera.SetPosition(glm::vec3(0, (CHUNK_SIZE_Y - MaxStructureY) - 2, 0));

		Logger::LogToConsole("The World was Constructed!");

		m_CrosshairTexture.CreateTexture("Resources/crosshair.png");
		m_CrosshairPosition = std::pair<float, float>((float)DEFAULT_WINDOW_X / 2, (float)DEFAULT_WINDOW_Y / 2);
		m_CurrentFrame = 0;
	}

	World::~World()
	{

	}

	void World::OnUpdate(GLFWwindow* window)
	{
		p_Player->OnUpdate();

		// Increase the frame count 
		m_CurrentFrame++;

		const float camera_speed = 0.35f;

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

		// Collision testing

		m_CloudManager.Update(glfwGetTime(), m_CurrentFrame);
	}

	void World::RenderWorld()
	{
		int player_chunk_x = 0;
		int player_chunk_y = 0;
		int player_chunk_z = 0;

		m_Skybox.RenderSkybox(&p_Player->p_Camera);

		player_chunk_x = floor(p_Player->p_Position.x / CHUNK_SIZE_X);
		player_chunk_z = floor(p_Player->p_Position.z / CHUNK_SIZE_Z);

		int render_distance_x = 2, render_distance_z = 2;
		std::uint32_t chunks_rendered = 0;

		int build_distance_x = render_distance_x + 2;
		int build_distance_z = render_distance_z + 2;

		/* 
		build_distance_x and build_distance_z is the build distance. It is needed to build 1 chunk 
		more than what is required in order for the meshing process to work properly
		
		For this it is required to first generate all the chunks that are needed to be generated. 
		Only after all the chunks are generated, the mesh building process with start
		*/

		for (int i = player_chunk_x - build_distance_x; i < player_chunk_x + build_distance_x; i++)
		{
			for (int j = player_chunk_z - build_distance_z; j < player_chunk_z + build_distance_z; j++)
			{
				if (ChunkExistsInMap(i, j) == false)
				{
					Chunk* chunk = GetChunkFromMap(i,j);
					GenerateChunk(chunk);
					chunk->p_MeshState = ChunkMeshState::Unbuilt;
				}
			}
		}

		// Render chunks according to render distance

		m_Renderer.StartChunkRendering(&p_Player->p_Camera);

		for (int i = player_chunk_x - render_distance_x; i < player_chunk_x + render_distance_x; i++)
		{
			for (int j = player_chunk_z - render_distance_z; j < player_chunk_z + render_distance_z; j++)
			{
				// Construct the chunks if the mesh isn't built

				Chunk* chunk = RetrieveChunkFromMap(i, j);

				if (chunk->p_MeshState == ChunkMeshState::Unbuilt)
				{
					ChunkMesh* mesh = chunk->GetChunkMesh();
 					mesh->ConstructMesh(chunk->m_ChunkContents, chunk->p_Position);
					chunk->p_MeshState = ChunkMeshState::Built;
				}

				else if (chunk->p_MeshState == ChunkMeshState::Edited)
				{
					UpdateSurroundingChunks(i, j);
				}

				// Render the chunks
				chunks_rendered++;
				RenderChunkFromMap(i, j);
			}
		}

		m_Renderer.EndChunkRendering();

		// Render the clouds
		m_CloudManager.RenderClouds(&p_Player->p_Camera);

		m_Renderer2D.RenderQuad(glm::vec3(m_CrosshairPosition.first - (m_CrosshairTexture.GetWidth() / 2)
			, m_CrosshairPosition.second - (m_CrosshairTexture.GetHeight() / 2), 1.0f)
			, &m_CrosshairTexture, &m_Camera2D);

	}

	void World::OnEvent(EventSystem::Event e)
	{
		p_Player->OnEvent(e);

		if (e.type == EventSystem::EventTypes::WindowResize)
		{
			float aspect = (float)e.wx / (float)e.wy;
			p_Player->p_Camera.SetAspect(aspect);
			m_Camera2D.SetProjection((float)0, (float)e.wx, (float)0, (float)e.wy);
			m_CrosshairPosition = std::pair <float,float> (e.wx / 2, e.wy / 2);
		}

		if (e.type == EventSystem::EventTypes::MousePress)
		{
			switch (e.button)
			{
				case GLFW_MOUSE_BUTTON_LEFT :
				{
					RayCast(false);
					break;
				}

				case GLFW_MOUSE_BUTTON_RIGHT:
				{
					RayCast(true);
					break;
				}
			}
		}
	}

	// Gets a block from position
	std::pair<Block*, Chunk*> World::GetWorldBlockFromPosition(const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		Chunk* chunk = GetChunkFromMap(block_chunk_x, block_chunk_z);

		return { &chunk->m_ChunkContents->at(bx).at(by).at(bz), chunk };
	}

	// Sets a world block from position
	void World::SetWorldBlockFromPosition(BlockType type, const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		GetChunkFromMap(block_chunk_x, block_chunk_z)->SetBlock(type, glm::vec3(bx, by, bz));
	}

	// Returns the chunk* and block* from BLOCK position
	std::pair<Block*, Chunk*> World::GetWorldBlock(const glm::vec3& block_loc)
	{
		int block_chunk_x = static_cast<int>(floor(block_loc.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(block_loc.z / CHUNK_SIZE_Z));

		Chunk* chunk = GetChunkFromMap(block_chunk_x, block_chunk_z);
		return { &chunk->m_ChunkContents->at(block_loc.x).at(block_loc.y).at(block_loc.z), chunk };
	}

	// Returns the type of block at a position
	BlockType World::GetWorldBlockTypeFromPosition(const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		return static_cast<BlockType>(GetChunkFromMap(block_chunk_x, block_chunk_z)->m_ChunkContents->at(bx).at(by).at(bz).p_BlockType);
	}

	// Converts world pixel coordinates to world block position
	glm::vec3 World::ConvertPositionToWorldBlockPosition(const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		bx += block_chunk_x * CHUNK_SIZE_X;
		bz += block_chunk_z * CHUNK_SIZE_Y;

		return glm::vec3(bx, by, bz);
	}

	void World::RenderChunkFromMap(int cx, int cz)
	{
		m_Renderer.RenderChunk(RetrieveChunkFromMap(cx, cz));
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

			if (position.y >= 0 && position.y < CHUNK_SIZE_Y)
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

					if (position.y >= 0 && position.y < CHUNK_SIZE_Y)
					{
						edit_block = GetWorldBlockFromPosition(glm::vec3(position.x, position.y, position.z));

						if (place)
						{
							edit_block.first->p_BlockType = BlockType::Dirt;
						}

						else
						{
							edit_block.first->p_BlockType = BlockType::Air;
						}

						edit_block.second->p_MeshState = ChunkMeshState::Edited;
					}

					return;
				}
			}
		}
	}

	// Updates all the chunks to the right, left, front and back of a supplied chunk
	void World::UpdateSurroundingChunks(int cx, int cz)
	{
		Chunk* chunk;
		ChunkMesh* mesh;

		chunk = RetrieveChunkFromMap(cx, cz);
		mesh = chunk->GetChunkMesh();
		mesh->ConstructMesh(chunk->m_ChunkContents, chunk->p_Position);
		chunk->p_MeshState = ChunkMeshState::Built;

		chunk = RetrieveChunkFromMap(cx + 1, cz);
		mesh = chunk->GetChunkMesh();
		mesh->ConstructMesh(chunk->m_ChunkContents, chunk->p_Position);
		chunk->p_MeshState = ChunkMeshState::Built;

		chunk = RetrieveChunkFromMap(cx - 1, cz);
		mesh = chunk->GetChunkMesh();
		mesh->ConstructMesh(chunk->m_ChunkContents, chunk->p_Position);
		chunk->p_MeshState = ChunkMeshState::Built;

		chunk = RetrieveChunkFromMap(cx, cz + 1);
		mesh = chunk->GetChunkMesh();
		mesh->ConstructMesh(chunk->m_ChunkContents, chunk->p_Position);
		chunk->p_MeshState = ChunkMeshState::Built;

		chunk = RetrieveChunkFromMap(cx, cz - 1);
		mesh = chunk->GetChunkMesh();
		mesh->ConstructMesh(chunk->m_ChunkContents, chunk->p_Position);
		chunk->p_MeshState = ChunkMeshState::Built;
	}

	bool World::ChunkExistsInMap(int cx, int cz)
	{
		std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));

		if (chunk_exists == m_WorldChunks.end())
		{
			return false;
		}

		return true;
	}

	Chunk* World::RetrieveChunkFromMap(int cx, int cz)
	{
		return &m_WorldChunks.at(std::pair<int, int>(cx, cz));
	}

	// Emplaces a chunk in the map
	Chunk* World::GetChunkFromMap(int cx, int cz)
	{
		std::stringstream str;

		str << "Chunk Building ! X : " << cx << " | Z : " << cz;

		std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));

		if (chunk_exists == m_WorldChunks.end())
		{
			Timer timer(str.str());

			m_WorldChunks.emplace(std::pair<int, int>(cx, cz), *(new Chunk(glm::vec3(cx, 1, cz))));
			m_ChunkCount++;
		}

		return &m_WorldChunks.at(std::pair<int, int>(cx, cz));
	}
}