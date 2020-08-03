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
		static std::vector<GUIVertex> GUI_Vertices; // Vertex buffer for buttons, windows and other colored quads 
		
		// Rendering and texturing 
		std::map<char, std::array<GLfloat, 8>>* GUI_TextureCoordinateMap;
		static GLClasses::Texture* GUI_FontAtlas;
		static glm::mat4 GUI_ProjectionMatrix;
		static GLFWwindow* GUI_Window;

		/* OpenGL Specific Objects */
		static GLClasses::VertexBuffer* GUI_VBO;
		static GLClasses::VertexArray* GUI_VAO;
		static GLClasses::VertexBuffer* GUI_Text_VBO;
		static GLClasses::VertexArray* GUI_Text_VAO;
		static GLClasses::IndexBuffer* GUI_IBO;
		static GLClasses::Shader* GUI_TextShader;
		static GLClasses::Shader* GUI_Shader;

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
			GUI_TextureCoordinateMap = new std::map<char, std::array<GLfloat, 8>>;
			GUI_VBO = new GLClasses::VertexBuffer(GL_ARRAY_BUFFER);
			GUI_VAO = new GLClasses::VertexArray;
			GUI_Text_VBO = new GLClasses::VertexBuffer(GL_ARRAY_BUFFER);
			GUI_Text_VAO = new GLClasses::VertexArray;
			GUI_IBO = new GLClasses::IndexBuffer;
			GUI_TextShader = new GLClasses::Shader;
			GUI_Shader = new GLClasses::Shader;
			GUI_FontAtlas = new GLClasses::Texture;

			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			GUI_ProjectionMatrix = glm::ortho(0.0f, static_cast<float>(w), 0.0f, static_cast<float>(h));

			GUI_FontAtlas->CreateTexture("Resources/Fonts/16x16.png");
			GUI_Window = window;

			GLClasses::TextureAtlas FontAtlas(GUI_FontAtlas->GetWidth(), GUI_FontAtlas->GetHeight(), glyph_x, glyph_y);

			// Add the texture coordinates for A - Z
			char curr_char = 'A';

			for (int i = 0; i < 26; i++)
			{
				GUI_TextureCoordinateMap->emplace(curr_char, FontAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1)));
				curr_char++;
			}

			curr_char = 'a';

			for (int i = 26; i < 26 * 2; i++)
			{
				GUI_TextureCoordinateMap->emplace(curr_char, FontAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1)));
				curr_char++;
			}

			curr_char = '0';

			for (int i = 26 * 2; i < (26 * 2) + 10; i++)
			{
				GUI_TextureCoordinateMap->emplace(curr_char, FontAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1)));
				curr_char++;
			}

			/* ToDo : Generate texture coordinates for other symbols */

			// Generate the index buffer

			GLuint* IndexBuffer = nullptr;

			int index_size = (w * h) * 2;
			int index_offset = 0;

			IndexBuffer = new GLuint[index_size * 6];

			for (size_t i = 0; i < index_size; i += 6)
			{
				IndexBuffer[i] = 0 + index_offset;
				IndexBuffer[i + 1] = 1 + index_offset;
				IndexBuffer[i + 2] = 2 + index_offset;
				IndexBuffer[i + 3] = 2 + index_offset;
				IndexBuffer[i + 4] = 3 + index_offset;
				IndexBuffer[i + 5] = 0 + index_offset;

				index_offset = index_offset + 4;
			}

			GUI_IBO->BufferData(index_size * 6 * sizeof(GLuint), IndexBuffer, GL_STATIC_DRAW);

			delete[] IndexBuffer;
			
			// Set up all the opengl objects

			// Text VAO
			GUI_Text_VAO->Bind();
			GUI_IBO->Bind();
			GUI_Text_VBO->Bind();
			GUI_Text_VBO->VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
			GUI_Text_VBO->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
			GUI_Text_VAO->Unbind();
			
			// Button VAO
			GUI_VAO->Bind();
			GUI_IBO->Bind();
			GUI_VBO->Bind();
			GUI_VBO->VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
			GUI_VBO->VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
			GUI_VAO->Unbind();

			// Set up all the UI Shaders
			GUI_Shader->CreateShaderProgramFromFile("Shaders/UI/UIElementVert.glsl", "Shaders/UI/UIElementFrag.glsl");
			GUI_TextShader->CreateShaderProgramFromFile("Shaders/UI/UITextVert.glsl", "Shaders/UI/UITextFrag.glsl");
			GUI_Shader->CompileShaders();
			GUI_TextShader->CompileShaders();

			return;
		}

		void GenerateButtonQuad(const glm::vec2& position, const glm::vec2& button_size)
		{
			// button_colors[0] is for an unhovered button and button_colors[1] is for an hovered button
			static glm::vec4 button_colors[2] = { glm::vec4(0.23f, 0.66f, 0.97f, 1.0f), 
												  glm::vec4(0.57f, 0.78f, 0.92f, 1.0f) };
			GUIVertex v1, v2, v3, v4;
			float x, y, w, h;
			glm::vec4 color = button_colors[0];

			// Check if the button is hovered or not
			double mx = 0, my = 0;
			int wx, wy;

			glfwGetCursorPos(GUI_Window, &mx, &my);
			glfwGetFramebufferSize(GUI_Window, &wx, &wy);

			GUI_AABB2D mouse_aabb = { (float)mx, (float)wy - my, 4, 4};
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

			GUI_Vertices.push_back(v1);
			GUI_Vertices.push_back(v2);
			GUI_Vertices.push_back(v3);
			GUI_Vertices.push_back(v4);
		}

		void GenerateGlyphs(const glm::vec2& position, const std::string& text)
		{
			float x_ = position.x;

			for (int i = 0; i < text.size(); i++)
			{
				const std::array<GLfloat, 8>& texture_coordinates = GUI_TextureCoordinateMap->at(text.at(i));
				GUITextVertex v1, v2, v3, v4;
				float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;

				x = x_;
				y = position.y;
				w = x + glyph_x;
				h = y + glyph_y;

				v1.position = glm::vec2(w, y);
				v2.position = glm::vec2(x, y);
				v3.position = glm::vec2(x, h);
				v4.position = glm::vec2(w, h);

				// Write the texture coordinates for each vertex
				v1.texture_coords = glm::vec2(texture_coordinates[0], texture_coordinates[1]);
				v2.texture_coords = glm::vec2(texture_coordinates[2], texture_coordinates[3]);
				v3.texture_coords = glm::vec2(texture_coordinates[4], texture_coordinates[5]);
				v4.texture_coords = glm::vec2(texture_coordinates[6], texture_coordinates[7]);

				GUI_TextVertices.push_back(v1);
				GUI_TextVertices.push_back(v2);
				GUI_TextVertices.push_back(v3);
				GUI_TextVertices.push_back(v4);

				x_ += glyph_x;
			}
		}

		bool Button(const glm::vec2& position, const glm::ivec2& button_size, const std::string& label)
		{
			GenerateGlyphs(glm::vec2(position.x + 4, position.y + 4), label);
			GenerateButtonQuad(glm::vec2(position.x - 4, position.y - 4), glm::vec2(button_size.x + 10, button_size.y + 10));
	
			return false;
		}

		void RenderUI(double ts, long long frame)
		{
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);

			int w, h;
			glfwGetFramebufferSize(GUI_Window, &w, &h);
			GUI_ProjectionMatrix = glm::ortho(0.0f, (float)w, 0.0f, (float)h);

			GUI_Shader->Use();
			GUI_Shader->SetMatrix4("u_ProjectionMatrix", GUI_ProjectionMatrix, 0);
			GUI_VAO->Bind();
			GUI_VBO->BufferData(GUI_Vertices.size() * sizeof(GUIVertex), &GUI_Vertices.front(), GL_STATIC_DRAW);
			glDrawElements(GL_TRIANGLES, floor(GUI_Vertices.size() / 4) * 6, GL_UNSIGNED_INT, 0);
			GUI_VAO->Unbind();

			GUI_TextShader->Use();
			GUI_FontAtlas->Bind(0);
			GUI_TextShader->SetMatrix4("u_ProjectionMatrix", GUI_ProjectionMatrix, 0);
			GUI_TextShader->SetInteger("u_FontTexture", 0, 0);
			GUI_Text_VAO->Bind();
			GUI_Text_VBO->BufferData(GUI_TextVertices.size() * sizeof(GUIVertex), &GUI_TextVertices.front(), GL_STATIC_DRAW);
			glDrawElements(GL_TRIANGLES, floor(GUI_TextVertices.size() / 4) * 6, GL_UNSIGNED_INT, 0);
			GUI_Text_VAO->Unbind();

			GUI_TextVertices.clear();
			GUI_Vertices.clear();
		}

		void UpdateUI(double ts, const glm::vec2& mouse_position, long long frame)
		{
			
		}

		void CloseUIContext()
		{
			delete GUI_VBO;
			delete GUI_Text_VBO;
			delete GUI_VAO;
			delete GUI_Text_VAO;
			delete GUI_IBO;
			delete GUI_FontAtlas;
			delete GUI_TextShader;
			delete GUI_Shader;
		
			GUI_TextVertices.clear();
			GUI_Vertices.clear();
			GUI_TextureCoordinateMap->clear();

			delete GUI_TextureCoordinateMap;
		}	

	}		 
}			
