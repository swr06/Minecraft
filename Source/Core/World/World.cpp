#include "World.h"

namespace Minecraft
{
	World::World()
	{

	}

	World::~World()
	{

	}

	void World::OnUpdate()
	{
		p_Player->OnUpdate();
	}

	void World::RenderWorld()
	{

	}
	void World::OnEvent()
	{
	}
}