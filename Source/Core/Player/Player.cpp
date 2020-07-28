#include "Player.h"

namespace Minecraft
{
	void Player::OnUpdate(GLFWwindow* window)
	{
		static FPSCamera cam(45, 1336/768, 0.0f, 99.0f);
		bool do_collision_check = false;
		const float camera_speed = 0.25;

		glm::vec3 prev_pos = p_Camera.GetPosition();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cam = p_Camera;
			cam.ChangePosition(glm::vec3(cam.GetFront().x * camera_speed, 0.0f, cam.GetFront().z * camera_speed));
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cam = p_Camera;
			cam.ChangePosition(-(glm::vec3(cam.GetFront().x * camera_speed, 0.0f, cam.GetFront().z * camera_speed)));
			do_collision_check = true;
		}
			
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cam = p_Camera;
			cam.ChangePosition(-(cam.GetRight() * camera_speed));
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cam = p_Camera;
			cam.ChangePosition(cam.GetRight() * camera_speed);
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			cam = p_Camera;
			cam.ChangePosition(cam.GetUp() * camera_speed);
			do_collision_check = true;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			cam = p_Camera;
			cam.ChangePosition(-(cam.GetUp() * camera_speed));
			do_collision_check = true;
		}

		if (do_collision_check)
		{
			if (TestBlockCollision(cam.GetPosition()) == false)
			{
				p_Camera = cam;
			}
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

	bool Player::TestBlockCollision(const glm::vec3& position)
	{
		glm::vec3 blockMin = p_PlayerAABB.GetRelativeMinimum(position);
		glm::vec3 blockMax = p_PlayerAABB.GetRelativeMaximum(position);

		if (position.y < CHUNK_SIZE_Y && blockMin.y < CHUNK_SIZE_Y && blockMax.y < CHUNK_SIZE_Y &&
			position.y >= 0 && blockMin.y >= 0 && blockMax.y >= 0)
		{
			blockMin = glm::vec3(floor(blockMin.x), floor(blockMin.y), floor(blockMin.z));
			blockMax = glm::vec3(floor(blockMax.x), floor(blockMax.y), floor(blockMax.z));

			for (int x = blockMin.x; x <= blockMax.x; ++x)
			{
				for (int y = blockMin.y; y <= blockMax.y; ++y)
				{
					for (int z = blockMin.z; z <= blockMax.z; ++z)
					{
						const Block* block = GetWorldBlock(glm::vec3(x, y, z));

						if (block && block->p_BlockType != BlockType::Air)
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