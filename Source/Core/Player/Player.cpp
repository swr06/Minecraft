#include "Player.h"

namespace Minecraft
{
	void Player::OnUpdate(GLFWwindow* window)
	{
		float camera_speed = 0.34;
		p_Position = p_Camera.GetPosition();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			p_Camera.ChangePosition(glm::vec3(p_Camera.GetFront().x * camera_speed, 0.0f, p_Camera.GetFront().z * camera_speed));

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			p_Camera.ChangePosition(-(glm::vec3(p_Camera.GetFront().x * camera_speed, 0.0f, p_Camera.GetFront().z * camera_speed)));

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			p_Camera.ChangePosition(-(p_Camera.GetRight() * camera_speed));

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			p_Camera.ChangePosition(p_Camera.GetRight() * camera_speed);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			p_Camera.ChangePosition(p_Camera.GetUp() * camera_speed);

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			p_Camera.ChangePosition(-(p_Camera.GetUp() * camera_speed));
		
		if (TestBlockCollision(p_Camera.GetPosition()))
		{
			std::cout << "\nCOLLISION!\n";
		}
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

		return false;
	}
}