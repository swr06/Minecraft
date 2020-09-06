#include <iostream>
#include <irrKlang.h>

#include <glm/glm.hpp> // glm::vec3 

namespace Minecraft
{
	namespace Audio
	{
		class Audio3D
		{
		public :
			Audio3D(const std::string& path, const glm::vec3& pos, irrklang::ISoundEngine* engine, bool loop = false) : p_Sound(nullptr), m_IsLoaded(false), m_Pth(path)
			{
				p_Sound = engine->play3D(path.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), loop, true);
				m_IsLoaded = true;
			}

			irrklang::ISound* p_Sound;

		private :
			std::string m_Pth;
			bool m_IsLoaded;
		};

		class Audio2D
		{
		public :
			Audio2D(const std::string& path, irrklang::ISoundEngine* engine, bool loop = false)
			{
				p_Sound = engine->play2D(path.c_str());
				m_Path = path;
			}

			irrklang::ISound* p_Sound;

		private :
			std::string m_Path;
		};
	}
}