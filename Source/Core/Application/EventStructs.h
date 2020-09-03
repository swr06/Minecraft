#include <iostream>
#include <glfw3.h>

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
			WindowResize,
			Undefined
		};

		struct Event
		{
			EventTypes type; // The type of event that has occured 

			GLFWwindow* window; // THe window in which the event occured
			int key; // The key that was pressed or released
			int button; // The mouse button that was pressed
			int mods; // The modifiers that were pressed with the key such as (CTRL, ALT. etc)
			int wx, wy; // Window Width and window height
			double mx, my; // Mouse X and Mouse Y
			double msx, msy; // Mouse scroll X and Mouse scroll y (The mouse scroll offset)
			double ts; // Event Timestep
		};
	}
}