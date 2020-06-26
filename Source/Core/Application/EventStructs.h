#include <iostream>

namespace Minecraft
{
	namespace EventSystem
	{
		enum EventTypes
		{
			KeyPress = 0,
			KeyRelease,
			MousePress,
			MouseRelease,
			MouseScroll,
			MouseMove,
			WindowResize
		};

		struct Event
		{
			EventTypes type;

			int key;
			int wx, wy; // Window Width and window height
			double mx, my; // Mouse X and Mouse Y
			double msx, msy; // Mouse scroll X and Mouse scroll y (The mouse scroll offset)
			double ts; // Event Timestep
		};
	}
}