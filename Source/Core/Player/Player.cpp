#include "Player.h"

namespace Minecraft
{
	void Player::OnUpdate(GLFWwindow* window)
	{
		bool do_collision_check = false;
		const float camera_speed = 0.05f;

		p_Camera.ResetAcceleration();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			// Take the cross product of the camera's right and up.
			glm::vec3 front = -glm::cross(p_Camera.GetRight(), p_Camera.GetUp());
			p_Camera.ApplyAcceleration(front * camera_speed);
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 back = glm::cross(p_Camera.GetRight(), p_Camera.GetUp());
			p_Camera.ApplyAcceleration(back * camera_speed);
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(-(p_Camera.GetRight() * camera_speed));
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(p_Camera.GetRight() * camera_speed);
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(p_Camera.GetUp() * camera_speed);
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			p_Camera.ApplyAcceleration(-(p_Camera.GetUp() * camera_speed));
			do_collision_check = true;
		}

		p_Camera.OnUpdate();

		if (do_collision_check)
		{
			// Todo : CHECK COLLISIONS
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

	static bool TestCollision(const glm::vec3& pos_1, const glm::vec3& dim_1, const glm::vec3& pos_2, const glm::vec3& dim_2)
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
		glm::vec3 blockMin = p_PlayerAABB.GetRelativeMinimum(position);
		glm::vec3 blockMax = p_PlayerAABB.GetRelativeMaximum(position);

		if (position.y < CHUNK_SIZE_Y && blockMin.y < CHUNK_SIZE_Y && blockMax.y < CHUNK_SIZE_Y &&
			position.y >= 0 && blockMin.y >= 0 && blockMax.y >= 0)
		{
			for (int x = blockMin.x; x <= blockMax.x; ++x)
			{
				for (int y = blockMin.y; y <= blockMax.y; ++y)
				{
					for (int z = blockMin.z; z <= blockMax.z; ++z)
					{
						const Block* block = GetWorldBlock(glm::vec3(x, y, z));

						if (block && block->Collidable())
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}
}