#include "Frustum.h"

namespace Minecraft
{
	Frustum::Frustum(FPSCamera* camera)
	{
		m_FOV = camera->GetFov();
		m_Aspect = camera->GetAspect();
	
	}
}