#include "GUI.h"

namespace Minecraft
{
	namespace GUI
	{
		// Used to fill in the text vertex buffer
		struct GUITextVertex
		{
			glm::vec2 position;
			glm::vec2 texture_coords;
		};

		// To draw the quads 
		struct GUIVertex
		{
			glm::vec2 position;
			glm::vec4 color;
		};

		/* To Check if a button is hovered over by the mouse */
		struct GUI_AABB2D
		{
			float x;
			float y;
			float w;
			float h;
		};

		struct GUI_Button
		{
			glm::vec2 position;
			std::string label;
		};

		// Vertex buffers
		static std::vector<GUITextVertex> GUI_TextVertices; // Vertex buffer for glyphs
		static std::vector<GUIVertex> GUI_TextVertices; // Vertex buffer for buttons, windows and other colored quads 
		
		// Rendering and texturing 
		static std::unordered_map<char, std::array<GLfloat, 8>> GUI_TextureCoordinateMap;
		static GLClasses::Texture* GUI_Atlas;
		static glm::mat4 GUI_ProjectionMatrix;
		static GLFWwindow* GUI_Window;

		// Entity and component storage
		static std::vector<GUI_Button> GUI_Buttons;

		/* OpenGL Specific Objects */
		static GLClasses::VertexBuffer* GUI_VBO;
		static GLClasses::VertexArray* GUI_VAO;
		static GLClasses::VertexBuffer* GUI_Text_VBO;
		static GLClasses::VertexArray* GUI_Text_VAO;
		static GLClasses::IndexBuffer* GUI_IBO;

		const float glyph_x = 16.0f;
		const float glyph_y = 16.0f;

		bool CheckAABBCollision(GUI_AABB2D one, GUI_AABB2D two)
		{
			float one_width = one.x + one.w;
			float one_height = one.y + one.h;
			float two_width = two.x + two.w;
			float two_height = two.y + two.h;

			bool collisionX = one_width >= two.x && two_width >= one.x;
			bool collisionY = one_height >= two.y && two_height >= one.y;
			
			return collisionX && collisionY;
		}

		void InitUISystem(GLFWwindow* window)
		{
			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			GUI_ProjectionMatrix = glm::ortho(0.0f, static_cast<float>(w), 0.0f, static_cast<float>(h));

			GUI_Atlas = new GLClasses::Texture;
			GUI_Atlas->CreateTexture("Resources/Fonts/16x16.png");
			GUI_Window = window;

			GLClasses::TextureAtlas FontAtlas(GUI_Atlas->GetWidth(), GUI_Atlas->GetHeight(), glyph_x, glyph_y);

			// Add the texture coordinates for A - Z
			char curr_char = 'A';

			for (int i = 0; i < 26; i++)
			{
				GUI_TextureCoordinateMap[curr_char] = FontAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1));
				curr_char++;
			}

			curr_char = 'a';

			for (int i = 26; i < 26 * 2; i++)
			{
				GUI_TextureCoordinateMap[curr_char] = FontAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1));
				curr_char++;
			}

			curr_char = '0';

			for (int i = 26 * 2; i < (26 * 2) + 10; i++)
			{
				GUI_TextureCoordinateMap[curr_char] = FontAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1));
				curr_char++;
			}

			/* ToDo : Generate texture coordinates for other symbols */
		}

		void GenerateButtonQuad(const glm::vec2& position, const glm::vec2& button_size)
		{
			// button_colors[0] is for an unhovered button and button_colors[1] is for an hovered button
			static glm::vec4 button_colors[2] = { glm::vec4(60 / 255, 170 / 255, 230 / 255, 1.0f), 
												  glm::vec4(130 / 255, 200 / 255, 237 / 255, 1.0f) };
			GUIVertex v1, v2, v3, v4;
			float x, y, w, h;
			glm::vec4 color = button_colors[0];

			// Check if the button is hovered or not
			double mx = 0, my = 0;

			glfwGetCursorPos(GUI_Window, &mx, &my);

			GUI_AABB2D mouse_aabb = { (float)mx, (float)my, 4, 4};
			GUI_AABB2D button_aabb = { position.x, position.y, button_size.x, button_size.y };
			
			if (CheckAABBCollision(button_aabb, mouse_aabb))
			{
				color = button_colors[1];
			}

			x = position.x;
			y = position.y;
			w = button_size.x + position.x;
			h = button_size.y + position.y;

			v1.position = glm::vec2(w, y);
			v2.position = glm::vec2(w, h);
			v3.position = glm::vec2(x, h);
			v4.position = glm::vec2(x, y);
			v1.color = color;
			v2.color = color;
			v3.color = color;
			v4.color = color;
		}

		void GenerateGlyphs(const glm::vec2& position, const std::string& text)
		{
			float x_ = position.x;

			for (int i = 0; i < text.size(); i++)
			{
				const std::array<GLfloat, 8>& texture_coordinates = GUI_TextureCoordinateMap.at(text.at(i));
				GUITextVertex v1, v2, v3, v4;
				float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;

				x = x_;
				y = position.y;
				w = x + glyph_x;
				h = y + glyph_y;

				v1.position = glm::vec2(w, y);
				v2.position = glm::vec2(w, h);
				v3.position = glm::vec2(x, h);
				v4.position = glm::vec2(x, y);

				// Write the texture coordinates for each vertex
				v1.texture_coords = glm::vec2(texture_coordinates[0], texture_coordinates[1]);
				v2.texture_coords = glm::vec2(texture_coordinates[2], texture_coordinates[3]);
				v3.texture_coords = glm::vec2(texture_coordinates[4], texture_coordinates[5]);
				v4.texture_coords = glm::vec2(texture_coordinates[6], texture_coordinates[7]);

				x_ += glyph_x;
			}
		}

		bool Button(const glm::vec2& position, const glm::ivec2& button_size, const std::string& label)
		{
			GenerateGlyphs(glm::vec2(position.x + 4, position.y + 4), label);
			GenerateButtonQuad(glm::vec2(position.x - 4, position.y - 4), glm::vec2(button_size.x + 10, button_size.y + 10));
		}

		void RenderUI(double ts, long long frame)
		{

		}

		void UpdateUI(double ts, const glm::vec2& mouse_position, long long frame)
		{

		}
	}
}
