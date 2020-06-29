#include "BlockTextureManager.h"

namespace Minecraft
{
	const std::array<GLfloat, 8>& GetBlockTexture(BlockType block_type, BlockFaceType face_type)
	{
		static GLClasses::TextureAtlas BlockTextureAtlas("Core\\Resources\\block_atlas.png", 32, 32);
		static std::array<GLfloat, 8> DirtBlockTexture[4];
		static std::array<GLfloat, 8> StoneBlockTexture;
		static std::array<GLfloat, 8> CobblestoneBlockTexture;
		static std::array<GLfloat, 8> LeafBlockTexture;
		static std::array<GLfloat, 8> SandBlockTexture;
		static std::array<GLfloat, 8> WoodBlockTexture[2];

		static bool arrays_initialized = false;

		if (arrays_initialized == false)
		{
			arrays_initialized = true;
			DirtBlockTexture[0] = BlockTextureAtlas.Sample(glm::vec2(0, 0), glm::vec2(1, 1));
			DirtBlockTexture[1] = BlockTextureAtlas.Sample(glm::vec2(1, 1), glm::vec2(2, 2));
			DirtBlockTexture[2] = BlockTextureAtlas.Sample(glm::vec2(2, 2), glm::vec2(3, 3));
			DirtBlockTexture[3] = BlockTextureAtlas.Sample(glm::vec2(2, 2), glm::vec2(3, 3));

			StoneBlockTexture = BlockTextureAtlas.Sample(glm::vec2(3, 3), glm::vec2(4, 4));
			CobblestoneBlockTexture = BlockTextureAtlas.Sample(glm::vec2(4, 4), glm::vec2(5, 5));
			WoodBlockTexture[0] = BlockTextureAtlas.Sample(glm::vec2(5, 5), glm::vec2(6, 6));
			WoodBlockTexture[1] = BlockTextureAtlas.Sample(glm::vec2(6, 6), glm::vec2(7, 7));
			LeafBlockTexture = BlockTextureAtlas.Sample(glm::vec2(7, 7), glm::vec2(8, 8));
			SandBlockTexture = BlockTextureAtlas.Sample(glm::vec2(8, 8), glm::vec2(9, 9));
		}

		switch (block_type)
		{
			case BlockType::Dirt:
			{
				switch (face_type)
				{
				case BlockFaceType::top:
					return DirtBlockTexture[0];
					break;

				case BlockFaceType::left:
					return DirtBlockTexture[1];
					break;

				case BlockFaceType::right:
					return DirtBlockTexture[1];
					break;

				default:
					return DirtBlockTexture[1];
					break;
				}

				break;
			}

			case BlockType::Stone:
			{
				return StoneBlockTexture;
				break;
			}

			case BlockType::Wood :
			{
				return WoodBlockTexture[0];
				break;
			}

			case BlockType::Leaf :
			{
				return LeafBlockTexture;
				break;
			}

			default : 
			{
				return WoodBlockTexture[0];
				break;
			}
		}
	}
}