#include "Player.h"

namespace Minecraft
{
	void Player::OnUpdate()
	{
		p_Position = p_Camera.GetPosition();
	}

	void Player::OnEvent()
	{

	}
}