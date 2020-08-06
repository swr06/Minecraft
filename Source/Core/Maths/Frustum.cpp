#include "Frustum.h"

namespace Minecraft
{
	Frustum::Frustum(FPSCamera* camera)
	{
		m_FOV = camera->GetFov();
		m_Aspect = camera->GetAspect();
		m_NearPlane = camera->GetNearPlane();
		m_FarPlane = camera->GetFarPlane();

		// Set the position and direction of the camera
		m_Position = camera->GetPosition();
		m_Direction = camera->GetFront();

		m_Tangent = tan(glm::radians(camera->GetFov()) * 0.5f);
		m_NearHeight = m_NearPlane * m_Tangent;
		m_NearWidth = m_NearHeight * m_Aspect;
		m_FarHeight = m_FarPlane * m_Tangent;
		m_FarWidth = m_FarHeight * m_Aspect;
	}

	void Frustum::UpdateCameraFrustum()
	{

	}	
}