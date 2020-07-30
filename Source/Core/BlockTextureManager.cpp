#include "BlockTextureManager.h"

namespace Minecraft
{
	const std::array<GLfloat, 8>& GetBlockTexture(BlockType block_type, BlockFaceType face_type)
	{
		static GLClasses::TextureAtlas BlockTextureAtlas("Resources/BlockAtlasHighDef.png", 128, 128);
		
		// Misc
		static std::array<GLfloat, 8> DirtBlockTexture[4];
		static std::array<GLfloat, 8> SandBlockTexture;
		static std::array<GLfloat, 8> CactusBlockTexture[3];

		// Leaves
		static std::array<GLfloat, 8> OakLeafBlockTexture;
		static std::array<GLfloat, 8> SpruceLeafTexture;

		// Wood and planks
		static std::array<GLfloat, 8> OakLogTexture[2];
		static std::array<GLfloat, 8> OakPlankTexture;
		static std::array<GLfloat, 8> AcaciaPlankTexture;
		static std::array<GLfloat, 8> DarkOakPlankTexture;

		// Stone and brick
		static std::array<GLfloat, 8> BrickBlockTexture;
		static std::array<GLfloat, 8> StoneBlockTexture;
		static std::array<GLfloat, 8> CobblestoneBlockTexture;
		static std::array<GLfloat, 8> StoneBrickTexture;
		static std::array<GLfloat, 8> CarvedStoneTexture;

		// Lamp
		static std::array<GLfloat, 8> LampTexture[2]; // lamp on and lamp off

		// Wools
		static std::array<GLfloat, 8> WoolRedTexture;
		static std::array<GLfloat, 8> WoolGreenTexture;
		static std::array<GLfloat, 8> WoolBlueTexture;
		static std::array<GLfloat, 8> WoolYellowTexture;

		// Transparent blocks
		static std::array<GLfloat, 8> GlassWhiteTexture;

		/// ///

		static bool arrays_initialized = false;

		if (arrays_initialized == false)
		{
			arrays_initialized = true;
			DirtBlockTexture[0] = BlockTextureAtlas.Sample(glm::vec2(0, 0), glm::vec2(1, 1));
			DirtBlockTexture[1] = BlockTextureAtlas.Sample(glm::vec2(1, 1), glm::vec2(2, 2));
			DirtBlockTexture[2] = BlockTextureAtlas.Sample(glm::vec2(3, 3), glm::vec2(2, 2));
			DirtBlockTexture[3] = BlockTextureAtlas.Sample(glm::vec2(3, 3), glm::vec2(4, 4));
			StoneBlockTexture = BlockTextureAtlas.Sample(glm::vec2(4, 4), glm::vec2(5, 5));
			CobblestoneBlockTexture = BlockTextureAtlas.Sample(glm::vec2(5, 5), glm::vec2(6, 6));
			OakLogTexture[0] = BlockTextureAtlas.Sample(glm::vec2(6, 6), glm::vec2(7, 7));
			OakLogTexture[1] = BlockTextureAtlas.Sample(glm::vec2(7, 7), glm::vec2(8, 8));
			OakLeafBlockTexture = BlockTextureAtlas.Sample(glm::vec2(8, 8), glm::vec2(9, 9));
			SpruceLeafTexture = BlockTextureAtlas.Sample(glm::vec2(9, 9), glm::vec2(10, 10));
			SandBlockTexture = BlockTextureAtlas.Sample(glm::vec2(10, 10), glm::vec2(11, 11));
			CactusBlockTexture[0] = BlockTextureAtlas.Sample(glm::vec2(11, 11), glm::vec2(12, 12));
			CactusBlockTexture[2] = BlockTextureAtlas.Sample(glm::vec2(12, 12), glm::vec2(13, 13));
			CactusBlockTexture[1] = BlockTextureAtlas.Sample(glm::vec2(13, 13), glm::vec2(14, 14));
			OakPlankTexture = BlockTextureAtlas.Sample(glm::vec2(14, 14), glm::vec2(15, 15));
			AcaciaPlankTexture = BlockTextureAtlas.Sample(glm::vec2(15, 15), glm::vec2(16, 16));
			DarkOakPlankTexture = BlockTextureAtlas.Sample(glm::vec2(16, 16), glm::vec2(17, 17));
			BrickBlockTexture = BlockTextureAtlas.Sample(glm::vec2(17, 17), glm::vec2(18, 18));
			StoneBrickTexture = BlockTextureAtlas.Sample(glm::vec2(18, 18), glm::vec2(19, 19));
			CarvedStoneTexture = BlockTextureAtlas.Sample(glm::vec2(19, 19), glm::vec2(20, 20));
			GlassWhiteTexture = BlockTextureAtlas.Sample(glm::vec2(20, 20), glm::vec2(21, 21));
			LampTexture[0] = BlockTextureAtlas.Sample(glm::vec2(21, 21), glm::vec2(22, 22));
			LampTexture[1] = BlockTextureAtlas.Sample(glm::vec2(22, 22), glm::vec2(23, 23));
			WoolRedTexture = BlockTextureAtlas.Sample(glm::vec2(23, 23), glm::vec2(24, 24));
			WoolGreenTexture = BlockTextureAtlas.Sample(glm::vec2(24, 24), glm::vec2(25, 25));
			WoolBlueTexture = BlockTextureAtlas.Sample(glm::vec2(25, 25), glm::vec2(26, 26));
			WoolYellowTexture = BlockTextureAtlas.Sample(glm::vec2(26, 26), glm::vec2(27, 27));
		}

		switch (block_type)
		{
		case BlockType::Grass:
		{
			switch (face_type)
			{
			case BlockFaceType::top:
				return DirtBlockTexture[0];
				break;

			case BlockFaceType::bottom:
				return DirtBlockTexture[1];
				break;

			case BlockFaceType::left:
				return DirtBlockTexture[2];
				break;

			case BlockFaceType::right:
				return DirtBlockTexture[2];
				break;

			case BlockFaceType::front:
				return DirtBlockTexture[3];
				break;

			case BlockFaceType::backward:
				return DirtBlockTexture[3];
				break;

			default:
				return DirtBlockTexture[1];
				break;
			}

			break;
		}

		case BlockType::Dirt:
		{
			return DirtBlockTexture[1];
			break;
		}

		case BlockType::Stone:
		{
			return StoneBlockTexture;
			break;
		}

		case BlockType::OakLog:
		{
			return OakLogTexture[0];
			break;
		}

		case BlockType::OakLeaves:
		{
			return OakLeafBlockTexture;
			break;
		}

		case BlockType::Sand:
		{
			return SandBlockTexture;
			break;
		}

		case BlockType::Cactus:
		{
			switch (face_type)
			{
				case BlockFaceType::top:
					return CactusBlockTexture[2];
					break;

				case BlockFaceType::bottom:
					return CactusBlockTexture[1];
					break;

				case BlockFaceType::left:
					return CactusBlockTexture[0];
					break;

				case BlockFaceType::right:
					return CactusBlockTexture[0];
					break;

				case BlockFaceType::front:
					return CactusBlockTexture[0];
					break;

				case BlockFaceType::backward:
					return CactusBlockTexture[0];
					break;

				default:
					return CactusBlockTexture[0];
					break;
			}
		}

		case BlockType::SpruceLeaves : 
		{
			return SpruceLeafTexture;
			break;
		}

		case BlockType::OakPlanks :
		{
			return OakPlankTexture;
			break;
		}

		case BlockType::AcaciaPlanks :
		{
			return AcaciaPlankTexture;
			break;
		}

		case BlockType::DarkOakPlanks : 
		{
			return DarkOakPlankTexture;
			break;
		}

		case BlockType::Bricks : 
		{
			return BrickBlockTexture;
			break;
		}

		case BlockType::StoneBricks : 
		{
			return StoneBrickTexture;
			break;
		}

		case BlockType::CarvedStone : 
		{
			return CarvedStoneTexture;
			break;
		}

		case BlockType::GlassWhite : 
		{
			return GlassWhiteTexture;
			break;
		}

		case BlockType::Lamp_On : 
		{
			return LampTexture[1];
			break;
		}

		case BlockType::Lamp_Off :
		{
			return LampTexture[0];
			break;
		}

		case BlockType::WoolRed : 
		{
			return WoolRedTexture;
			break;
		}

		case BlockType::WoolGreen:
		{
			return WoolGreenTexture;
			break;
		}

		case BlockType::WoolBlue:
		{
			return WoolBlueTexture;
			break;
		}

		case BlockType::WoolYellow:
		{
			return WoolYellowTexture;
			break;
		}
		}
	}
}