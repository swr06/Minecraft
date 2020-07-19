#include "Player.h"

namespace Minecraft
{
	void Player::OnUpdate()
	{
		p_Position = p_Camera.GetPosition();

		// Update the AABB mask
		p_PlayerAABB.x = p_Position.x;
		p_PlayerAABB.y = p_Position.y;
		p_PlayerAABB.z = p_Position.z;
	}

	void Player::OnEvent(EventSystem::Event e)
	{
		if (e.type == EventSystem::EventTypes::MouseScroll)
		{
			if (e.msy > 0.0f)
			{
				p_Camera.SetFov(p_Camera.GetFov() + 0.1);
			}

			else if (e.msy < 0.0f)
			{
				p_Camera.SetFov(p_Camera.GetFov() - 0.1);
			}
		}

		else if (e.type == EventSystem::EventTypes::MouseMove)
		{
			p_Camera.UpdateOnMouseMovement(e.mx, e.my);
		}
	}
}