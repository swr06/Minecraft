#include "Player.h"

namespace Minecraft
{
	void Player::OnUpdate(GLFWwindow* window)
	{
		const float camera_speed = 0.05f;

		p_Camera.ResetAcceleration();
		FPSCamera cam = p_Camera;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			// Take the cross product of the camera's right and up.
			glm::vec3 front = -glm::cross(p_Camera.GetRight(), p_Camera.GetUp());
			p_Camera.ApplyAcceleration(front * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 back = glm::cross(p_Camera.GetRight(), p_Camera.GetUp());
			p_Camera.ApplyAcceleration(back * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(-(p_Camera.GetRight() * camera_speed));
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(p_Camera.GetRight() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(p_Camera.GetUp() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(-(p_Camera.GetUp() * camera_speed));
		}

		p_Camera.OnUpdate();
		glm::vec3 new_pos = p_Camera.GetPosition();
		glm::vec3 old_pos = cam.GetPosition();

		if (new_pos != old_pos)
		{
			glm::vec3* camera_pos = (glm::vec3*)&p_Camera.GetPosition();

			if (TestBlockCollision(glm::vec3(new_pos.x, old_pos.y, old_pos.z )))
			{
				camera_pos->x = old_pos.x;
				p_Camera.ResetVelocity();
				p_Camera.ResetAcceleration();
			}

			if (TestBlockCollision(glm::vec3(old_pos.x, old_pos.y, new_pos.z)))
			{
				camera_pos->z = old_pos.z;
				p_Camera.ResetVelocity();
				p_Camera.ResetAcceleration();
			}

			if (TestBlockCollision(glm::vec3(old_pos.x, new_pos.y, old_pos.z)))
			{
				camera_pos->y = old_pos.y;
				p_Camera.ResetVelocity();
				p_Camera.ResetAcceleration();
			}

			p_Camera.Refresh();
		}

		// Update the player's position
		p_Position = p_Camera.GetPosition();
	}

	void Player::OnEvent(EventSystem::Event e)
	{
		if (e.type == EventSystem::EventTypes::MouseScroll)
		{
			if (e.msy > 0.0f)
			{
				if (p_Camera.GetFov() < 71)
				{
					p_Camera.SetFov(p_Camera.GetFov() + 0.1);
				}
			}

			else if (e.msy < 0.0f)
			{
				if (p_Camera.GetFov() > 69.50)
				{
					p_Camera.SetFov(p_Camera.GetFov() - 0.1);
				}
			}
		}

		else if (e.type == EventSystem::EventTypes::KeyPress)
		{
			if (e.key == GLFW_KEY_F)
			{
				p_FreeFly = !p_FreeFly;
			}
		}

		else if (e.type == EventSystem::EventTypes::MouseMove)
		{
			p_Camera.UpdateOnMouseMovement(e.mx, e.my);
		}
	}

	static bool Test3DAABBCollision(const glm::vec3& pos_1, const glm::vec3& dim_1, const glm::vec3& pos_2, const glm::vec3& dim_2)
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

	bool Player::TestBlockCollision(const glm::vec3& position)
	{
		// Convert center position to top-left position
		glm::vec3 pos = glm::vec3(
			position.x - 0.375f,
			position.y - 0.75,
			position.z - 0.375f);

		glm::ivec3 player_block = {
			(int)floor(pos.x),
			(int)floor(pos.y),
			(int)floor(pos.z)
		};

		const glm::ivec3 block_range = { 2, 2, 2 };

		for (int i = player_block.x - block_range.x; i < player_block.x + block_range.x; i++)
			for (int j = player_block.y - block_range.y; j < player_block.y + block_range.y; j++)
				for (int k = player_block.z - block_range.z; k < player_block.z + block_range.z; k++)
				{
					if (j < CHUNK_SIZE_Y && j >= 0)
					{
						Block* block = GetWorldBlock(glm::vec3(i, j, k));

						if (block && block->Collidable())
						{
							if (Test3DAABBCollision(pos, glm::vec3(0.75f, 1.5f, 0.75f), glm::vec3(i, j, k), glm::vec3(1, 1, 1)))
							{
								return true;
							}
						}
					}
				}

		return false;

	}
}