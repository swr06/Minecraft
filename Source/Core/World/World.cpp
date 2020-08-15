#include "World.h"

namespace Minecraft
{
	// The amount of chunks that gets rendered around the player
	const int render_distance = 6;
	const int render_distance_x = render_distance, render_distance_z = render_distance;

	static void PrintVec3(const glm::vec3& val)
	{
		std::cout << std::endl << "X : " << val.x << " Y : " << val.y << " Z : " << val.z;
		return;
	}

	bool TestCollision(const glm::vec3& pos_1, const glm::vec3& dim_1, const glm::vec3& pos_2, const glm::vec3& dim_2)
	{
		if (pos_1.x < pos_2.x + dim_2.x &&
			pos_1.x + dim_1.x > pos_2.x &&
			pos_1.y < pos_2.y + dim_2.y &&
			pos_1.y + dim_1.y > pos_2.y &&
			pos_1.z < pos_2.z + dim_2.z &&
			pos_1.z + dim_1.z > pos_2.z)
		{
			return true;
		}

		return false;
	}

	// Converts world block coordinates to local block coordinates or chunk coordinates. Used for lighting calculations
	static glm::ivec3 WorldBlockToLocalBlockCoordinates(const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		return glm::ivec3(bx, by, bz);
	}

	World::World(int seed, const glm::vec2& window_size, const std::string& world_name, WorldGenerationType world_gen_type) 
		: m_Camera2D(0.0f, window_size.x, 0.0f, window_size.y), m_WorldSeed(seed), m_WorldName(world_name), m_WorldGenType(world_gen_type)
	{
		m_SunCycle = CurrentSunCycle::Sun_Rising;
		m_SunPosition = glm::vec4(0.0f, 700.0f, 0.0f, 1.0f);

		// Generate all the chunks 

		p_Player = new Player(window_size.x, window_size.y);

		// Set the players position
		p_Player->p_Camera.SetPosition(glm::vec3(0, 140, 0));

		Logger::LogToConsole("The World was Constructed!");

		m_CrosshairTexture.CreateTexture("Resources/crosshair.png");
		
		float cw = floor(static_cast<float>(window_size.x) / static_cast <float>(2.0f));
		float cy = floor(static_cast<float>(window_size.y) / static_cast<float>(2.0f));

		m_CrosshairPosition = std::pair<float, float>(cw, cy);
		m_CurrentFrame = 0;
	}

	World::~World()
	{

	}

	void World::OnUpdate(GLFWwindow* window)
	{
		int player_chunk_x = (int)floor(p_Player->p_Position.x / CHUNK_SIZE_X);
		int player_chunk_z = (int)floor(p_Player->p_Position.z / CHUNK_SIZE_Z);

		std::uint32_t chunks_rendered = 0;
		std::stringstream str;

		int build_distance_x = render_distance_x + 4;
		int build_distance_z = render_distance_z + 4;

		/*
		build_distance_x and build_distance_z is the build distance. It is needed to build `x` chunks
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
					Chunk* chunk = EmplaceChunkInMap(i, j);
					GenerateChunk(chunk, m_WorldSeed, m_WorldGenType);
					chunk->p_MeshState = ChunkMeshState::Unbuilt;
				}
			}
		}

		p_Player->OnUpdate(window);

		// Update the view frustum
		m_ViewFrustum.Update(p_Player->p_Camera.GetViewProjection());

		// Increase the frame count 
		m_CurrentFrame++;

		// Collision testing
	}

	void World::RenderWorld()
	{
		static float ambient = 0.4f;
		int player_chunk_x = 0;
		int player_chunk_z = 0;

		glDisable(GL_CULL_FACE);
		m_Skybox.RenderSkybox(&p_Player->p_Camera, m_SunPosition);
		
		// Enable face culling and depth testing
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);

		// Determine the player's current chunk
		player_chunk_x = (int)floor(p_Player->p_Position.x / CHUNK_SIZE_X);
		player_chunk_z = (int)floor(p_Player->p_Position.z / CHUNK_SIZE_Z);
		uint32_t chunks_rendered = 0;

		// Render chunks according to render distance

		m_Renderer.StartChunkRendering(&p_Player->p_Camera, glm::vec4(ambient, ambient, ambient, 1.0f), render_distance, m_SunPosition);

		for (int i = player_chunk_x - render_distance_x; i < player_chunk_x + render_distance_x; i++)
		{
			for (int j = player_chunk_z - render_distance_z; j < player_chunk_z + render_distance_z; j++)
			{
				Chunk* chunk = RetrieveChunkFromMap(i, j);

				if (m_ViewFrustum.BoxInFrustum(chunk->p_ChunkFrustumAABB))
				{
					// Construct the chunks if the mesh isn't built
					if (chunk->p_MeshState == ChunkMeshState::Unbuilt)
					{
						chunk->Construct();
					}

					else if (chunk->p_MeshState == ChunkMeshState::Edited)
					{
						UpdateSurroundingChunks(i, j);
					}

					m_Renderer.RenderChunk(chunk);

					// Render the chunks
					chunks_rendered++;
				}
			}
		}

		glDisable(GL_CULL_FACE);

		for (int i = player_chunk_x - render_distance_x; i < player_chunk_x + render_distance_x; i++)
		{
			for (int j = player_chunk_z - render_distance_z; j < player_chunk_z + render_distance_z; j++)
			{
				Chunk* chunk = RetrieveChunkFromMap(i, j);

				if (m_ViewFrustum.BoxInFrustum(chunk->p_ChunkFrustumAABB))
				{
					m_Renderer.RenderTransparentChunk(chunk);
				}
			}
		}

		m_Renderer.EndChunkRendering();

		glDisable(GL_CULL_FACE);

		m_Renderer.StartChunkModelRendering(&p_Player->p_Camera, glm::vec4(ambient, ambient, ambient, 1.0f), render_distance, m_SunPosition);

		for (int i = player_chunk_x - render_distance_x; i < player_chunk_x + render_distance_x; i++)
		{
			for (int j = player_chunk_z - render_distance_z; j < player_chunk_z + render_distance_z; j++)
			{
				Chunk* chunk = RetrieveChunkFromMap(i, j);

				if (m_ViewFrustum.BoxInFrustum(chunk->p_ChunkFrustumAABB))
				{
					m_Renderer.RenderChunkModels(chunk);
				}
			}
		}

		m_Renderer.EndChunkModelRendering();

		glDisable(GL_DEPTH_TEST);

		m_Renderer2D.RenderQuad(glm::vec3(m_CrosshairPosition.first - (m_CrosshairTexture.GetWidth() / 2)
			, m_CrosshairPosition.second - (m_CrosshairTexture.GetHeight() / 2), 1.0f)
			, &m_CrosshairTexture, &m_Camera2D);

		// Do collision tests after all rendering is complete
		DoCollisionTests();

		// Tick the sun every 8 frames 
		if (m_CurrentFrame % 8 == 0)
		{
			TickSun();
		}
	}

	void World::TickSun()
	{
		const float max_sun = 1500.0f;
		const float min_sun = 10.0f;

		if (m_SunCycle == CurrentSunCycle::Sun_Rising)
		{
			if (m_SunPosition.y >= max_sun)
			{
				m_SunPosition.y = max_sun;
				m_SunCycle = CurrentSunCycle::Sun_Setting;

				return;
			}

			m_SunPosition.y += 1;
		}

		else if (m_SunCycle == CurrentSunCycle::Sun_Setting)
		{
			if (m_SunPosition.y <= min_sun)
			{
				m_SunPosition.y = min_sun;
				m_SunCycle = CurrentSunCycle::Sun_Rising;

				return;
			}

			m_SunPosition.y -= 1;
		}
	}

	void World::OnEvent(EventSystem::Event e)
	{
		p_Player->OnEvent(e);

		if (e.type == EventSystem::EventTypes::WindowResize)
		{
			if (e.wx > 0 && e.wy > 0)
			{
				float aspect = (float)e.wx / (float)e.wy;
				p_Player->p_Camera.SetAspect(aspect);
				m_Camera2D.SetProjection((float)0, (float)e.wx, (float)0, (float)e.wy);
				m_CrosshairPosition = std::pair <float, float>(e.wx / 2, e.wy / 2);
			}
		}

		if (e.type == EventSystem::EventTypes::KeyPress && e.key == GLFW_KEY_Q)
		{
			m_CurrentHeldBlock++;

			if (m_CurrentHeldBlock == BlockType::UnknownBlockType)
			{
				m_CurrentHeldBlock = 0;
			}
		}

		else if (e.type == EventSystem::EventTypes::MousePress)
		{
			switch (e.button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
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
	std::pair<Block*, Chunk*> World::GetBlockFromPosition(const glm::vec3& pos) noexcept
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		Chunk* chunk = RetrieveChunkFromMap(block_chunk_x, block_chunk_z);

		return { &chunk->p_ChunkContents.at(bx).at(by).at(bz), chunk };
	}

	// Sets a world block from position
	void World::SetBlockFromPosition(BlockType type, const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		RetrieveChunkFromMap(block_chunk_x, block_chunk_z)->SetBlock(type, glm::vec3(bx, by, bz));
	}

	// Returns the chunk* and block* from BLOCK position
	std::pair<Block*, Chunk*> World::GetBlock(const glm::vec3& block_loc) noexcept
	{
		int block_chunk_x = static_cast<int>(floor(block_loc.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(block_loc.z / CHUNK_SIZE_Z));

		Chunk* chunk = RetrieveChunkFromMap(block_chunk_x, block_chunk_z);
		return { &chunk->p_ChunkContents.at(block_loc.x).at(block_loc.y).at(block_loc.z), chunk };
	}

	// Returns the type of block at a position
	BlockType World::GetBlockTypeFromPosition(const glm::vec3& pos) noexcept
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		return static_cast<BlockType>(RetrieveChunkFromMap(block_chunk_x, block_chunk_z)->p_ChunkContents.at(bx).at(by).at(bz).p_BlockType);
	}

	// Converts world pixel coordinates to world block position
	glm::vec3 World::ConvertPositionToWorldBlockPosition(const glm::vec3& pos) noexcept
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
				std::pair<Block*, Chunk*> ray_hitblock = GetBlockFromPosition(glm::vec3(
					floor(position.x),
					floor(position.y),
					floor(position.z)
				));

				Block* ray_block = ray_hitblock.first;
				Chunk* ray_chunk = ray_hitblock.second;

				if (ray_block != nullptr && ray_block->p_BlockType != BlockType::Air && ray_block->IsLiquid() == false)
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
						auto& player_pos = p_Player->p_Position;
						edit_block = GetBlockFromPosition(glm::vec3(position.x, position.y, position.z));

						if (place && TestCollision(position, glm::vec3(1, 1, 1), p_Player->p_Position, glm::vec3(1, 2, 1)) == false)
						{
							edit_block.first->p_BlockType = static_cast<BlockType>(m_CurrentHeldBlock);

							if (static_cast<BlockType>(m_CurrentHeldBlock) == BlockType::Lamp_On)
							{
								glm::ivec3 light_block = WorldBlockToLocalBlockCoordinates(position);

								edit_block.second->SetTorchLightAt(light_block.x, light_block.y, light_block.z, 16);

								// Push it to the light bfs
								m_LightBFSQueue.push({ glm::vec3(light_block.x, light_block.y, light_block.z), edit_block.second });

								// Do the lighting calculations
								UpdateLights();
							}
						}

						else
						{
							// If the block was a light block, do the light calculations again and push it to the light removal bfs queue
							if (edit_block.first->p_BlockType == BlockType::Lamp_On)
							{
								glm::ivec3 light_removal_block = WorldBlockToLocalBlockCoordinates(position);

								m_LightRemovalBFSQueue.push({ glm::vec3(light_removal_block.x, light_removal_block.y, light_removal_block.z),
									edit_block.second->GetTorchLightAt(light_removal_block.x, light_removal_block.y, light_removal_block.z),
									edit_block.second });

								edit_block.second->SetTorchLightAt(light_removal_block.x, light_removal_block.y, light_removal_block.z, 0);
								UpdateLights();
							}

							edit_block.first->p_BlockType = BlockType::Air;
						}

						// Set the chunk mesh state 
						edit_block.second->p_MeshState = ChunkMeshState::Edited;
						edit_block.second->p_ChunkState = ChunkState::Changed;
					}

					return;
				}
			}
		}
	}

	void World::DoCollisionTests()
	{


	}

	// Updates all the chunks to the right, left, front and back of a supplied chunk
	void World::UpdateSurroundingChunks(int cx, int cz)
	{
		Chunk* chunk;
		ChunkMesh* mesh;

		chunk = RetrieveChunkFromMap(cx + 1, cz);
		chunk->Construct();

		chunk = RetrieveChunkFromMap(cx - 1, cz);
		chunk->Construct();

		chunk = RetrieveChunkFromMap(cx, cz + 1);
		chunk->Construct();

		chunk = RetrieveChunkFromMap(cx, cz - 1);
		chunk->Construct();

		chunk = RetrieveChunkFromMap(cx, cz);
		chunk->Construct();
	}

	void World::UpdateLights()
	{
		// Light removal bfs queue

		while (m_LightRemovalBFSQueue.empty() == false)
		{
			LightRemovalNode& node = m_LightRemovalBFSQueue.front();
			int x = node.p_Position.x;
			int y = node.p_Position.y;
			int z = node.p_Position.z;
			int light_level = (int)node.p_LightValue;
			glm::vec3 chunk_pos = node.p_Chunk->p_Position;
			Chunk* chunk = node.p_Chunk;

			Chunk* front_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.z + 1);
			Chunk* back_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.z - 1);
			Chunk* right_chunk = RetrieveChunkFromMap(chunk_pos.x + 1, chunk_pos.z);
			Chunk* left_chunk = RetrieveChunkFromMap(chunk_pos.x - 1, chunk_pos.z);

			// For lighting on chunk corners
			Chunk* tr_chunk = RetrieveChunkFromMap(chunk_pos.x + 1, chunk_pos.z + 1); // Top right chunk
			Chunk* tl_chunk = RetrieveChunkFromMap(chunk_pos.x - 1, chunk_pos.z + 1); // Top left chunk
			Chunk* br_chunk = RetrieveChunkFromMap(chunk_pos.x + 1, chunk_pos.z - 1); // Bottom right chunk
			Chunk* bl_chunk = RetrieveChunkFromMap(chunk_pos.x - 1, chunk_pos.z - 1); // Bottom left chunk

			// Pop the front element
			m_LightRemovalBFSQueue.pop();

			if (x > 0)
			{
				int neighbor_level = chunk->GetTorchLightAt(x - 1, y, z);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					chunk->SetTorchLightAt(x - 1, y, z, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x - 1, y, z), neighbor_level, chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x - 1, y, z), chunk);
				}
			}

			else if (x == 0)
			{
				int neighbor_level = left_chunk->GetTorchLightAt(CHUNK_SIZE_X - 1, y, z);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					left_chunk->SetTorchLightAt(CHUNK_SIZE_X - 1, y, z, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(CHUNK_SIZE_X - 1, y, z), neighbor_level, left_chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(CHUNK_SIZE_X - 1, y, z), left_chunk);
				}
			}

			if (x < CHUNK_SIZE_X - 1)
			{
				int neighbor_level = chunk->GetTorchLightAt(x + 1, y, z);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					chunk->SetTorchLightAt(x + 1, y, z, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x + 1, y, z), neighbor_level, chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x + 1, y, z), chunk);
				}
			}

			else if (x == CHUNK_SIZE_X - 1)
			{
				int neighbor_level = right_chunk->GetTorchLightAt(0, y, z);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					right_chunk->SetTorchLightAt(0, y, z, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(0, y, z), neighbor_level, right_chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(0, y, z), right_chunk);
				}
			}

			if (y > 0)
			{
				int neighbor_level = chunk->GetTorchLightAt(x, y - 1, z);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					chunk->SetTorchLightAt(x, y - 1, z, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x, y - 1, z), neighbor_level, chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x, y - 1, z), chunk);
				}
			}

			if (y < CHUNK_SIZE_Y - 1)
			{
				int neighbor_level = chunk->GetTorchLightAt(x, y + 1, z);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					chunk->SetTorchLightAt(x, y + 1, z, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x, y + 1, z), neighbor_level, chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x, y + 1, z), chunk);
				}
			}

			if (z > 0)
			{
				int neighbor_level = chunk->GetTorchLightAt(x, y, z - 1);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					chunk->SetTorchLightAt(x, y, z - 1, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x, y, z - 1), neighbor_level, chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x, y, z - 1), chunk);
				}
			}

			else if (z == 0)
			{
				int neighbor_level = back_chunk->GetTorchLightAt(x, y, CHUNK_SIZE_Z - 1);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					back_chunk->SetTorchLightAt(x, y, CHUNK_SIZE_Z - 1, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x, y, CHUNK_SIZE_Z - 1), neighbor_level, back_chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x, y, CHUNK_SIZE_Z - 1), back_chunk);
				}
			}

			if (z < CHUNK_SIZE_Z - 1)
			{
				int neighbor_level = chunk->GetTorchLightAt(x, y, z + 1);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					chunk->SetTorchLightAt(x, y, z + 1, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x, y, z + 1), neighbor_level, chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x, y, z + 1), chunk);
				}
			}

			else if (z == CHUNK_SIZE_Z - 1)
			{
				int neighbor_level = front_chunk->GetTorchLightAt(x, y, 0);

				if (neighbor_level != 0 && neighbor_level < light_level)
				{
					front_chunk->SetTorchLightAt(x, y, 0, 0);
					m_LightRemovalBFSQueue.emplace(glm::vec3(x, y, 0), neighbor_level, front_chunk);
				}

				else if (neighbor_level >= light_level)
				{
					m_LightBFSQueue.emplace(glm::vec3(x, y, 0), front_chunk);
				}
			}
		}

		while (!m_LightBFSQueue.empty())
		{
			LightNode& node = m_LightBFSQueue.front();
			glm::vec3 pos = node.p_Position;
			Chunk* chunk = node.p_Chunk;

			// Pop the element after storing it's data
			m_LightBFSQueue.pop();

			int light_level = chunk->GetTorchLightAt(pos.x, pos.y, pos.z);
			int x = floor(pos.x);
			int y = floor(pos.y);
			int z = floor(pos.z);

			glm::vec3 chunk_pos = chunk->p_Position;
			Chunk* front_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.z + 1);
			Chunk* back_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.z - 1);
			Chunk* right_chunk = RetrieveChunkFromMap(chunk_pos.x + 1, chunk_pos.z);
			Chunk* left_chunk = RetrieveChunkFromMap(chunk_pos.x - 1, chunk_pos.z);

			// For lighting on chunk corners

			if (x > 0)
			{
				if (chunk->GetBlock(x - 1, y, z)->p_BlockType != BlockType::Air && chunk->GetTorchLightAt(x - 1, y, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x - 1, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x - 1, y, z), chunk });
				}
			}

			else if (x == 0)
			{
				if (left_chunk->GetBlock(CHUNK_SIZE_X - 1, y, z)->p_BlockType != BlockType::Air && left_chunk->GetTorchLightAt(CHUNK_SIZE_X - 1, y, z) + 2 <= light_level)
				{
					left_chunk->SetTorchLightAt(CHUNK_SIZE_X - 1, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(CHUNK_SIZE_X - 1, y, z), left_chunk });
				}
			}

			if (x < CHUNK_SIZE_X - 1)
			{
				if (chunk->GetBlock(x + 1, y, z)->p_BlockType != BlockType::Air && chunk->GetTorchLightAt(x + 1, y, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x + 1, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x + 1, y, z), chunk });
				}
			}

			else if (x == CHUNK_SIZE_X - 1)
			{
				if (right_chunk->GetBlock(0, y, z)->p_BlockType != BlockType::Air && right_chunk->GetTorchLightAt(0, y, z) + 2 <= light_level)
				{
					right_chunk->SetTorchLightAt(0, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(0, y, z), right_chunk });
				}
			}

			if (y > 0)
			{
				if (chunk->GetBlock(x, y - 1, z)->p_BlockType != BlockType::Air && chunk->GetTorchLightAt(x, y - 1, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y - 1, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y - 1, z), chunk });
				}
			}

			if (y < CHUNK_SIZE_Y - 1)
			{
				if (chunk->GetBlock(x, y + 1, z)->p_BlockType != BlockType::Air && chunk->GetTorchLightAt(x, y + 1, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y + 1, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y + 1, z), chunk });
				}
			}

			if (z > 0)
			{
				if (chunk->GetBlock(x, y, z - 1)->p_BlockType != BlockType::Air && chunk->GetTorchLightAt(x, y, z - 1) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y, z - 1, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, z - 1), chunk });

				}
			}

			else if (z == 0)
			{
				if (back_chunk->GetBlock(x, y, CHUNK_SIZE_Z - 1)->p_BlockType != BlockType::Air && back_chunk->GetTorchLightAt(x, y, CHUNK_SIZE_Z - 1) + 2 <= light_level)
				{
					back_chunk->SetTorchLightAt(x, y, CHUNK_SIZE_Z - 1, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, CHUNK_SIZE_Z - 1), back_chunk });
				}
			}

			if (z < CHUNK_SIZE_Z - 1)
			{
				if (chunk->GetBlock(x, y, z + 1)->p_BlockType != BlockType::Air && chunk->GetTorchLightAt(x, y, z + 1) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y, z + 1, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, z + 1), chunk });
				}
			}

			else if (z == CHUNK_SIZE_Z - 1)
			{
				if (front_chunk->GetBlock(x, y, 0)->p_BlockType != BlockType::Air && front_chunk->GetTorchLightAt(x, y, 0) + 2 <= light_level)
				{
					front_chunk->SetTorchLightAt(x, y, 0, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, 0), front_chunk });
				}
			}
		}
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

	// Gets an existing chunk from the map
	Chunk* World::RetrieveChunkFromMap(int cx, int cz) noexcept
	{
		Chunk* ret_val = &m_WorldChunks.at(std::pair<int, int>(cx, cz));
		return ret_val;
	}

	// Emplaces a chunk in the map
	Chunk* World::EmplaceChunkInMap(int cx, int cz)
	{
		std::stringstream str;

		str << "Chunk Building ! X : " << cx << " | Z : " << cz;

		std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));

		if (chunk_exists == m_WorldChunks.end())
		{
			Timer timer(str.str());

			m_WorldChunks.emplace(std::pair<int, int>(cx, cz), glm::vec3(cx, 0, cz));
			m_ChunkCount++;
		}

		return &m_WorldChunks.at(std::pair<int, int>(cx, cz));
	}

	int World::GetSeed()
	{
		return m_WorldSeed;
	}
}