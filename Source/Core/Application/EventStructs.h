#include <iostream>

namespace Minecraft
{
	namespace EventSystem
	{
		enum EventTypes
		{
			KeyPress,
			KeyRelease,
			MousePress,
			MouseRelease,
			MouseScroll,
			WindowResize
		};

		struct Event
		{
			EventTypes type;

			int wx, wy; // Window Width and window height
			double mx, my; // Mouse X and Mouse Y
			double msx, msy; // Mouse scroll X and Mouse scroll y (The mouse scroll offset)
			double ts; // Event Timestep
		};
	}
}