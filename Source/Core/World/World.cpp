#include "World.h"

namespace Minecraft
{
	// The amount of chunks that gets rendered around the player
	const int render_distance = 6;
	const int render_distance_x = render_distance, render_distance_z = render_distance;

	constexpr float max_sun = 1500.0f;
	constexpr float min_sun = 10.0f;
	constexpr uint8_t LAMP_LIGHT_LEVEL = 32;

	/*
		Prints a 3 component vector on the screen
	*/

	static void PrintVec3(const glm::vec3& val)
	{
		std::cout << std::endl << "X : " << val.x << " Y : " << val.y << " Z : " << val.z;
		return;
	}

	/*
		Tests the collision between a block and a position.
	*/
	bool TestAABB3DCollision(const glm::vec3& pos_1, const glm::vec3& dim_1, const glm::vec3& pos_2, const glm::vec3& dim_2)
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

	/*
		Converts world position to chunk block position
	*/

	static glm::ivec3 WorldBlockToLocalBlockCoordinates(const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		return glm::ivec3(bx, by, bz);
	}


	/*
		World class constructor
	*/

	World::World(int seed, const glm::vec2& window_size, const std::string& world_name, WorldGenerationType world_gen_type)
		: m_Camera2D(0.0f, window_size.x, 0.0f, window_size.y), m_WorldSeed(seed), m_WorldName(world_name), m_WorldGenType(world_gen_type)
	{
		m_SunCycle = CurrentSunCycle::Sun_Rising;
		m_SunPosition = glm::vec4(0.0f, max_sun, 0.0f, 1.0f);

		// Generate all the chunks 

		p_Player = new Player(window_size.x, window_size.y);

		// Set the players position
		p_Player->p_Position = glm::vec3(0, 140, 0);
		p_Player->p_Camera.SetPosition(p_Player->p_Position);

		Logger::LogToConsole("The World was Constructed!");

		m_CrosshairTexture.CreateTexture("Resources/crosshair.png");

		float cw = floor(static_cast<float>(window_size.x) / static_cast <float>(2.0f));
		float cy = floor(static_cast<float>(window_size.y) / static_cast<float>(2.0f));

		m_CrosshairPosition = std::pair<float, float>(cw, cy);
		m_CurrentFrame = 0;

		std::cout << std::endl << "------      CREATING THE AUDIO ENGINE        ------" << std::endl;

		// Create the sound engine
		m_SoundEngine = irrklang::createIrrKlangDevice();

		std::cout << std::endl;
	}

	World::~World()
	{

	}


	/*
		The World::OnUpdate function is called every frame, it updates the blocks, lighting, player etc..
	*/
	void World::OnUpdate(GLFWwindow* window)
	{
		int player_chunk_x = (int)floor(p_Player->p_Position.x / CHUNK_SIZE_X);
		int player_chunk_z = (int)floor(p_Player->p_Position.z / CHUNK_SIZE_Z);

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
				}
			}
		}

		int flora_build_distance_x = build_distance_x - 1;
		int flora_build_distance_z = build_distance_z - 1;

		for (int i = player_chunk_x - flora_build_distance_x; i < player_chunk_x + flora_build_distance_x; i++)
		{
			for (int j = player_chunk_z - flora_build_distance_z; j < player_chunk_z + flora_build_distance_z; j++)
			{
				Chunk* chunk = RetrieveChunkFromMap(i, j);

				if (chunk->p_ChunkState == ChunkState::Ungenerated)
				{
					GenerateChunkFlora(chunk, m_WorldSeed, m_WorldGenType);
					chunk->p_MeshState = ChunkMeshState::Unbuilt;
					chunk->p_ChunkState = ChunkState::Generated;
				}

			}
		}

		p_Player->OnUpdate(window);

		// Update the view frustum
		m_ViewFrustum.Update(p_Player->p_Camera.GetViewProjection());

		// Increase the frame count 
		m_CurrentFrame++;

		// Clean up the particles every x frames
		if (m_CurrentFrame % 300 == 0)
		{
			m_ParticleEmitter.CleanUpList();
		}

		// Update the listeners position
		_SetListenerPosition();
	}

	/*
		World::Render world is called to render the world.
		Called every frame
	*/
	void World::RenderWorld()
	{
		static float ambient = 0.4f;
		int player_chunk_x = 0;
		int player_chunk_z = 0;

		glDisable(GL_CULL_FACE);
		m_Skybox.RenderSkybox(&p_Player->p_Camera, m_SunPosition);

		// Enable face culling and depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

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

					if (chunk->p_MeshState == ChunkMeshState::Built)
					{
						m_Renderer.RenderChunk(chunk);
					}

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

		// Tick the sun every x frames 
		if (m_CurrentFrame % 8 == 0)
		{
			TickSun();
		}

		m_ParticleEmitter.OnUpdateAndRender(&p_Player->p_Camera, m_Renderer.GetAtlasTexture());

		/* TEMPORARY */
		//m_CubeRenderer.RenderCube(glm::vec3(0, 135, 0), &m_CrosshairTexture, 0, p_Player->p_Camera.GetViewProjection(), glm::mat4(1.0f), nullptr);
	}

	/*
		The tick sun ticks the sun by one unit.
		This function is called every 'x' frames

	*/

	void World::TickSun()
	{

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

	/*
		Called by Application when an even takes place
		Called every frame
	*/
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

		if (e.type == EventSystem::EventTypes::KeyPress && e.key == GLFW_KEY_E)
		{
			p_Player->p_CurrentHeldBlock++;

			if (p_Player->p_CurrentHeldBlock == BlockType::Water)
			{
				p_Player->p_CurrentHeldBlock = BlockType::Grass;
			}
		}

		if (e.type == EventSystem::EventTypes::KeyPress && e.key == GLFW_KEY_Q)
		{
			if (p_Player->p_CurrentHeldBlock > BlockType::Grass)
			{
				p_Player->p_CurrentHeldBlock--;
			}
		}

		if (e.type == EventSystem::EventTypes::KeyPress && e.key == GLFW_KEY_G)
		{
			m_SunPosition.y = max_sun;
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

	/*
		Gets a block from position.
		Returns : The block and chunk (of that position)
	*/
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

	/*
		Sets a world block to the type parameter
	*/
	void World::SetBlockFromPosition(BlockType type, const glm::vec3& pos)
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		RetrieveChunkFromMap(block_chunk_x, block_chunk_z)->SetBlock(type, glm::vec3(bx, by, bz));
	}

	/*
		Returns the type of the block at a particular position
	*/
	BlockType World::GetBlockTypeFromPosition(const glm::vec3& pos) noexcept
	{
		int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
		int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
		int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
		int by = static_cast<int>(floor(pos.y));
		int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

		return static_cast<BlockType>(RetrieveChunkFromMap(block_chunk_x, block_chunk_z)->p_ChunkContents.at(bx).at(by).at(bz).p_BlockType);
	}

	/*
		Unloads the chunks when it is too faraway.
		Called every 200~ frames
		TODO
	*/
	void World::UnloadFarChunks()
	{


		return;
	}

	/*
		Tests whether the placed block intersects with the player
		Used to make sure that the player doesnt get stuck in a block that the player places
	*/
	bool World::TestRayPlayerCollision(const glm::vec3& ray_block)
	{
		glm::vec3 pos = glm::vec3(
			p_Player->p_Position.x,
			p_Player->p_Position.y,
			p_Player->p_Position.z);

		if (TestAABB3DCollision(pos, glm::vec3(0.75f, 1.5f, 0.75f), ray_block, glm::vec3(1.0f, 1.0f, 1.0f)))
		{
			return true;
		}

		return false;
	}

	/*
	Plays the sound for a given block
	*/
	void World::_PlayBlockSound(BlockType type, const glm::vec3& position)
	{
		const std::string& snd = BlockDatabase::GetBlockSoundPath(type);

		if (snd.size() > 0)
		{
			Audio::Audio3D aud(snd, position, m_SoundEngine, false);
			aud.p_Sound->setMinDistance(20.0f);
			aud.p_Sound->setVolume(10.0f);
			aud.p_Sound->setIsPaused(false);
		}
	}

	/*
	Sets the listners position
	*/
	void World::_SetListenerPosition()
	{
		const glm::vec3& position = p_Player->p_Position;
		const glm::vec3& front = p_Player->p_Camera.GetFront();
		const glm::vec3& up = p_Player->p_Camera.GetUp();
		irrklang::vec3df _position(position.x, position.y, position.z);
		irrklang::vec3df _vps(0, 0, 0); // not really needed
		irrklang::vec3df _front(front.x, front.y, front.z);
		irrklang::vec3df _up(up.x, up.y, up.z);

		m_SoundEngine->setListenerPosition(_position, _front, _vps, _up);
	}

	/*
		Casts a ray from the direction (front) vector of the camera.
		Speed : Fast
		"place" determines whether the block should be places (bool)
	*/
	void World::RayCast(bool place)
	{
		glm::vec3 position = p_Player->p_Position;
		const glm::vec3& direction = p_Player->p_Camera.GetFront();
		int max = 50; // block reach

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
						edit_block = GetBlockFromPosition(glm::vec3(position.x, position.y, position.z));
						glm::ivec3 local_block_pos = WorldBlockToLocalBlockCoordinates(position);
						glm::vec2 chunk_pos = glm::vec2(edit_block.second->p_Position.x, edit_block.second->p_Position.z);

						Chunk* front_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.y + 1);
						Chunk* back_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.y - 1);
						Chunk* right_chunk = RetrieveChunkFromMap(chunk_pos.x + 1, chunk_pos.y);
						Chunk* left_chunk = RetrieveChunkFromMap(chunk_pos.x - 1, chunk_pos.y);

						BlockType snd_type;

						if (place && !TestRayPlayerCollision(position))
						{
							/*
								We need to update the light when ever a block is placed or remove.
								Add the surrounding blocks to the removal bfs queue
							*/

							m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z),
							edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y, local_block_pos.z),
							edit_block.second });

							if (local_block_pos.x == 0)
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(CHUNK_SIZE_X - 1, local_block_pos.y, local_block_pos.z),
									edit_block.second->GetTorchLightAt(CHUNK_SIZE_X - 1, local_block_pos.y, local_block_pos.z),
									left_chunk });
							}

							else
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x - 1, local_block_pos.y, local_block_pos.z),
									edit_block.second->GetTorchLightAt(local_block_pos.x - 1, local_block_pos.y, local_block_pos.z),
									edit_block.second });
							}

							if (local_block_pos.x == CHUNK_SIZE_X - 1)
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(0, local_block_pos.y, local_block_pos.z),
									edit_block.second->GetTorchLightAt(0, local_block_pos.y, local_block_pos.z),
									right_chunk });
							}

							else
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x + 1, local_block_pos.y, local_block_pos.z),
									edit_block.second->GetTorchLightAt(local_block_pos.x + 1, local_block_pos.y, local_block_pos.z),
									edit_block.second });
							}

							if (local_block_pos.z == 0)
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, CHUNK_SIZE_Z - 1),
									edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y, CHUNK_SIZE_Z - 1),
									back_chunk });
							}

							else
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z - 1),
								edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y, local_block_pos.z - 1),
								edit_block.second });
							}

							if (local_block_pos.z == CHUNK_SIZE_Z - 1)
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, 0),
									edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y, 0),
									front_chunk });
							}

							else
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z + 1),
								edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y, local_block_pos.z + 1),
								edit_block.second });
							}

							if (local_block_pos.y > 0)
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y - 1, local_block_pos.z),
									edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y - 1, local_block_pos.z),
									edit_block.second });
							}

							if (local_block_pos.y < CHUNK_SIZE_Y - 1)
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y + 1, local_block_pos.z),
									edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y + 1, local_block_pos.z),
									edit_block.second });
							}

							/* Lighting calculations end here */

							edit_block.first->p_BlockType = static_cast<BlockType>(p_Player->p_CurrentHeldBlock);
							snd_type = edit_block.first->p_BlockType;

							if (static_cast<BlockType>(p_Player->p_CurrentHeldBlock) == BlockType::Lamp_On)
							{
								edit_block.second->SetTorchLightAt(local_block_pos.x, local_block_pos.y, local_block_pos.z, LAMP_LIGHT_LEVEL);

								// Push it to the light bfs
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z), edit_block.second });
							}

							// Do the lighting calculations
							UpdateLights();
						}

						else
						{
							/*
								We need to update the light when ever a block is removed.
								Add the surrounding blocks to the removal bfs queue
								We also need to check for chunk bounds
							*/

							m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z), edit_block.second });

							if (local_block_pos.x == 0)
							{
								m_LightBFSQueue.push({ glm::vec3(CHUNK_SIZE_X - 1, local_block_pos.y, local_block_pos.z), left_chunk });
							}

							else
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x - 1, local_block_pos.y, local_block_pos.z), edit_block.second });
							}

							if (local_block_pos.x == CHUNK_SIZE_X - 1)
							{
								m_LightBFSQueue.push({ glm::vec3(0, local_block_pos.y, local_block_pos.z), right_chunk });
							}
							else
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x + 1, local_block_pos.y, local_block_pos.z), edit_block.second });
							}

							if (local_block_pos.z == 0)
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, CHUNK_SIZE_Z - 1), back_chunk });
							}

							else
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z - 1), edit_block.second });
							}

							if (local_block_pos.z == CHUNK_SIZE_Z - 1)
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, 0), front_chunk });
							}

							else
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z + 1), edit_block.second });
							}

							if (local_block_pos.y > 0)
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y - 1, local_block_pos.z), edit_block.second });
							}

							if (local_block_pos.y < CHUNK_SIZE_Y - 1)
							{
								m_LightBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y + 1, local_block_pos.z), edit_block.second });
							}

							snd_type = edit_block.first->p_BlockType;

							/* Lighting calculations end here */

							if (edit_block.first->p_BlockType == BlockType::Lamp_On)
							{
								m_LightRemovalBFSQueue.push({ glm::vec3(local_block_pos.x, local_block_pos.y, local_block_pos.z),
									edit_block.second->GetTorchLightAt(local_block_pos.x, local_block_pos.y, local_block_pos.z),
									edit_block.second });

								edit_block.second->SetTorchLightAt(local_block_pos.x, local_block_pos.y, local_block_pos.z, 0);
							}

							else if (edit_block.first->p_BlockType == BlockType::Bedrock)
							{
								return;
							}

							/*
							If there is a flower or other model above destroy that too.
							*/
							if (local_block_pos.y >= 0 && local_block_pos.y < CHUNK_SIZE_Y - 1)
							{
								if (edit_block.second->p_ChunkContents.at(local_block_pos.x).at(local_block_pos.y + 1).at(local_block_pos.z).DependsOnBelowBlock())
								{
									edit_block.second->p_ChunkContents.at(local_block_pos.x).at(local_block_pos.y + 1).at(local_block_pos.z).p_BlockType = BlockType::Air;
								}
							}

							// Emit particles at the block position

							if (edit_block.first->p_BlockType != BlockType::Air)
							{
								glm::vec3 particle_pos;
								particle_pos.x = floor(position.x);
								particle_pos.y = position.y;
								particle_pos.z = floor(position.z);

								particle_pos.x += 0.5f;
								particle_pos.z += 0.5f;

								m_ParticleEmitter.EmitParticlesAt(10, 40,
									particle_pos, glm::vec3(5, 5, 5), glm::vec3(0.06f, 1, 0.06f), edit_block.first->p_BlockType);
							}

							edit_block.first->p_BlockType = BlockType::Air;
							UpdateLights();
						}

						edit_block.second->p_MeshState = ChunkMeshState::Unbuilt;

						/*
						Check if the edited block was on one of the chunk edges, if it was change the respective neighbouring chunk's mesh state
						*/
						if (local_block_pos.x <= 0)
						{
							Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->p_Position.x - 1, edit_block.second->p_Position.z);
							update_chunk->p_MeshState = ChunkMeshState::Unbuilt;
						}

						if (local_block_pos.z <= 0)
						{
							Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->p_Position.x, edit_block.second->p_Position.z - 1);
							update_chunk->p_MeshState = ChunkMeshState::Unbuilt;
						}

						if (local_block_pos.x >= CHUNK_SIZE_X - 1)
						{
							Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->p_Position.x + 1, edit_block.second->p_Position.z);
							update_chunk->p_MeshState = ChunkMeshState::Unbuilt;
						}

						if (local_block_pos.z >= CHUNK_SIZE_Z - 1)
						{
							Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->p_Position.x, edit_block.second->p_Position.z + 1);
							update_chunk->p_MeshState = ChunkMeshState::Unbuilt;
						}

						edit_block.second->p_ChunkState = ChunkState::Changed;

						/* Play the block sound */
						_PlayBlockSound(snd_type, position);
					}

					return;
				}
			}
		}
	}

	/*
		Goes through the bfs queue and propogates the light through the air blocks in the required chunks.
	*/
	void World::PropogateLight()
	{
		while (!m_LightBFSQueue.empty())
		{
			LightNode& node = m_LightBFSQueue.front();
			glm::vec3 pos = node.p_Position;
			Chunk* chunk = node.p_Chunk;

			// Pop the element after storing it's data
			m_LightBFSQueue.pop();

			int light_level = 0;

			if (pos.x >= 0 && pos.x < CHUNK_SIZE_X &&
				pos.z >= 0 && pos.z < CHUNK_SIZE_Z &&
				pos.y >= 0 && pos.y < CHUNK_SIZE_Y)
			{
				light_level = chunk->GetTorchLightAt(pos.x, pos.y, pos.z);
			}

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
				if (chunk->GetBlock(x - 1, y, z)->IsLightPropogatable() && chunk->GetTorchLightAt(x - 1, y, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x - 1, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x - 1, y, z), chunk });
				}

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			else if (x <= 0)
			{
				if (left_chunk->GetBlock(CHUNK_SIZE_X - 1, y, z)->IsLightPropogatable() && left_chunk->GetTorchLightAt(CHUNK_SIZE_X - 1, y, z) + 2 <= light_level)
				{
					left_chunk->SetTorchLightAt(CHUNK_SIZE_X - 1, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(CHUNK_SIZE_X - 1, y, z), left_chunk });
				}

				left_chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			if (x < CHUNK_SIZE_X - 1)
			{
				if (chunk->GetBlock(x + 1, y, z)->IsLightPropogatable() && chunk->GetTorchLightAt(x + 1, y, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x + 1, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x + 1, y, z), chunk });
				}

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			else if (x >= CHUNK_SIZE_X - 1)
			{
				if (right_chunk->GetBlock(0, y, z)->IsLightPropogatable() && right_chunk->GetTorchLightAt(0, y, z) + 2 <= light_level)
				{
					right_chunk->SetTorchLightAt(0, y, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(0, y, z), right_chunk });
				}

				right_chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			if (y > 0)
			{
				if (chunk->GetBlock(x, y - 1, z)->IsLightPropogatable() && chunk->GetTorchLightAt(x, y - 1, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y - 1, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y - 1, z), chunk });
				}

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			if (y < CHUNK_SIZE_Y - 1)
			{
				if (chunk->GetBlock(x, y + 1, z)->IsLightPropogatable() && chunk->GetTorchLightAt(x, y + 1, z) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y + 1, z, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y + 1, z), chunk });
				}

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			if (z > 0)
			{
				if (chunk->GetBlock(x, y, z - 1)->IsLightPropogatable() && chunk->GetTorchLightAt(x, y, z - 1) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y, z - 1, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, z - 1), chunk });
				}

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			else if (z <= 0)
			{
				if (back_chunk->GetBlock(x, y, CHUNK_SIZE_Z - 1)->IsLightPropogatable() && back_chunk->GetTorchLightAt(x, y, CHUNK_SIZE_Z - 1) + 2 <= light_level)
				{
					back_chunk->SetTorchLightAt(x, y, CHUNK_SIZE_Z - 1, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, CHUNK_SIZE_Z - 1), back_chunk });
				}

				back_chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			if (z < CHUNK_SIZE_Z - 1)
			{
				if (chunk->GetBlock(x, y, z + 1)->IsLightPropogatable() && chunk->GetTorchLightAt(x, y, z + 1) + 2 <= light_level)
				{
					chunk->SetTorchLightAt(x, y, z + 1, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, z + 1), chunk });
				}

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}

			else if (z >= CHUNK_SIZE_Z - 1)
			{
				if (front_chunk->GetBlock(x, y, 0)->IsLightPropogatable() && front_chunk->GetTorchLightAt(x, y, 0) + 2 <= light_level)
				{
					front_chunk->SetTorchLightAt(x, y, 0, light_level - 1);
					m_LightBFSQueue.push({ glm::vec3(x, y, 0), front_chunk });
				}

				front_chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}
		}
	}

	/*
	Goes through the bfs queue and removes the required lights at the required chunks
	*/
	void World::RemoveLight()
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

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				left_chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				right_chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				back_chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				chunk->p_MeshState = ChunkMeshState::Unbuilt;
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

				front_chunk->p_MeshState = ChunkMeshState::Unbuilt;
			}
		}
	}

	/*
		Updates all the chunks
	*/
	void World::UpdateLights()
	{
		PropogateLight();
		RemoveLight();
		PropogateLight();
	}

	/*
		Checks if the chunk exists in the chunk map
	*/
	bool World::ChunkExistsInMap(int cx, int cz)
	{
		std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));

		if (chunk_exists == m_WorldChunks.end())
		{
			return false;
		}

		return true;
	}

	/*
		Gets a chunk if it exists in the chunk map
	*/
	Chunk* World::RetrieveChunkFromMap(int cx, int cz) noexcept
	{
		auto chk = m_WorldChunks.find(std::pair<int, int>(cx, cz));

		if (chk == m_WorldChunks.end())
		{
			std::stringstream ss;
			ss << "INVALID CHUNK REQUESTED !    CX : " << cx << "    CZ : " << cz;
			Logger::LogToConsole(ss.str());
			return nullptr;
		}

		Chunk* ret_val = &m_WorldChunks.at(std::pair<int, int>(cx, cz));
		return ret_val;
	}

	/*
		Emplaces a chunk in the chunk map
	*/
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

	/*
		Gets the world seed
	*/
	int World::GetSeed()
	{
		return m_WorldSeed;
	}
}