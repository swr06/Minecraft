#include "BlockDatabase.h"

namespace Minecraft
{
	/*
	A hacky way to map block faces to texture coordinates. :)
	Don't bother understanding it

	If it works, it works
	 ¯\_(ツ)_/¯
	*/

	namespace BlockDatabase
	{
		enum class BlockTypeTexture
		{
			CactusTop = -5,
			CactusBottom,
			OakLogTop,
			GrassTop,
			GrassFront,
			GrassSide, // Will be 0. The others can be casted to work with the BlockType enum. 
			Dirt,
			Stone,
			Cobblestone,
			StoneBricks,
			CarvedStone,
			OakLeaves,
			SpruceLeaves,
			OakLog,
			Cactus,
			Sand,
			OakPlanks,
			AcaciaPlanks,
			DarkOakPlanks,
			Bricks,
			GlassWhite,
			Lamp_On,
			Lamp_Off,
			WoolRed,
			WoolGreen,
			WoolBlue,
			WoolYellow,
			Gravel,
			Clay,
			Water,
			Lava,
			Snow,
			Slime,
			Model_Grass,
			Model_Deadbush,
			Model_Kelp,
			Flower_allium,
			Flower_orchid,
			Flower_tulip_red,
			Flower_tulip_pink,
			Flower_rose,
			Flower_dandelion,

			Bedrock,
			UnknownBlockType,
			Air
		};

		BlockTypeTexture GetRealBlockTexture(BlockType block_type, BlockFaceType face_type)
		{
			uint8_t block = (BlockType)block_type;
			BlockTypeTexture ret_val = (BlockTypeTexture)block;

			switch (block_type)
			{
			case BlockType::Grass:
			{
				if (face_type == BlockFaceType::front || face_type == BlockFaceType::backward)
					ret_val = BlockTypeTexture::GrassFront;

				else if (face_type == BlockFaceType::top)
					ret_val = BlockTypeTexture::GrassTop;

				else if (face_type == BlockFaceType::left || face_type == BlockFaceType::right)
					ret_val = BlockTypeTexture::GrassSide;

				else if (face_type == BlockFaceType::bottom)
					ret_val = BlockTypeTexture::Dirt;

				break;
			}

			case BlockType::OakLog:
			{
				if (face_type == BlockFaceType::top || face_type == BlockFaceType::bottom)
					ret_val = BlockTypeTexture::OakLogTop;

				else
					ret_val = BlockTypeTexture::OakLog;

				break;
			}

			case BlockType::Cactus:
			{
				if (face_type == BlockFaceType::top)
					ret_val = BlockTypeTexture::CactusTop;

				else if (face_type == BlockFaceType::bottom)
					ret_val = BlockTypeTexture::CactusBottom;

				else
					ret_val = BlockTypeTexture::Cactus;

				break;
			}
			}

			return ret_val;
		}

		std::unordered_map<BlockTypeTexture, std::array<GLfloat, 8>> BlockDatabase;
		std::unordered_map<BlockType, std::string> BlockNameDatabase;

		const std::array<GLfloat, 8>& GetBlockTexture(BlockType block_type, BlockFaceType face_type)
		{
			static bool first_run = true;

			if (first_run)
			{
				GLClasses::TextureAtlas BlockTextureAtlas("Resources/64x64_sheet.png", 64, 64);
				first_run = false;

				constexpr int list_sz = 41;

				BlockTypeTexture texture_list[list_sz] =
				{
					BlockTypeTexture::GrassTop,
					BlockTypeTexture::Dirt,
					BlockTypeTexture::GrassSide,
					BlockTypeTexture::GrassFront,
					BlockTypeTexture::Stone,
					BlockTypeTexture::Cobblestone,
					BlockTypeTexture::OakLog,
					BlockTypeTexture::OakLogTop,
					BlockTypeTexture::OakLeaves,
					BlockTypeTexture::SpruceLeaves,
					BlockTypeTexture::Sand,
					BlockTypeTexture::Cactus,
					BlockTypeTexture::CactusBottom,
					BlockTypeTexture::CactusTop,
					BlockTypeTexture::OakPlanks,
					BlockTypeTexture::AcaciaPlanks,
					BlockTypeTexture::DarkOakPlanks,
					BlockTypeTexture::Bricks,
					BlockTypeTexture::StoneBricks,
					BlockTypeTexture::CarvedStone,
					BlockTypeTexture::GlassWhite,
					BlockTypeTexture::Lamp_Off,
					BlockTypeTexture::Lamp_On,
					BlockTypeTexture::WoolRed,
					BlockTypeTexture::WoolGreen,
					BlockTypeTexture::WoolBlue,
					BlockTypeTexture::WoolYellow,
					BlockTypeTexture::Water,
					BlockTypeTexture::Gravel,
					BlockTypeTexture::Clay,
					BlockTypeTexture::Model_Grass,
					BlockTypeTexture::Model_Deadbush,
					BlockTypeTexture::Flower_allium,
					BlockTypeTexture::Flower_orchid,
					BlockTypeTexture::Flower_tulip_red,
					BlockTypeTexture::Flower_tulip_pink,
					BlockTypeTexture::Flower_rose,
					BlockTypeTexture::Flower_dandelion,
					BlockTypeTexture::Model_Kelp,
					BlockTypeTexture::Bedrock,
					BlockTypeTexture::UnknownBlockType
				};

				for (int i = 0; i < list_sz; i++)
				{
					if (texture_list[i] == BlockTypeTexture::GrassSide)
					{
						BlockDatabase[texture_list[i]] = BlockTextureAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1), true);
					}

					else
					{
						BlockDatabase[texture_list[i]] = BlockTextureAtlas.Sample(glm::vec2(i, i), glm::vec2(i + 1, i + 1));
					}
				}
			}

			BlockTypeTexture tex = GetRealBlockTexture(block_type, face_type);
			std::unordered_map<BlockTypeTexture, std::array<GLfloat, 8>>::iterator iter = BlockDatabase.find(tex);

			if (iter == BlockDatabase.end())
			{
				return BlockDatabase[BlockTypeTexture::UnknownBlockType];
			}

			return BlockDatabase[tex];
		}

		const std::string& GetBlockName(BlockType block_type)
		{
			static bool first_run = true;

			if (first_run)
			{
				first_run = false;
				BlockNameDatabase[BlockType::Grass] = "Grass";
				BlockNameDatabase[BlockType::Dirt] = "Dirt";
				BlockNameDatabase[BlockType::Stone] = "Stone";
				BlockNameDatabase[BlockType::Cobblestone] = "Cobblestone";
				BlockNameDatabase[BlockType::StoneBricks] = "Stone bricks";
				BlockNameDatabase[BlockType::CarvedStone] = "Carved/Chiseled Stone";
				BlockNameDatabase[BlockType::OakLeaves] = "Oak Leaves";
				BlockNameDatabase[BlockType::SpruceLeaves] = "Spruce Leaves";
				BlockNameDatabase[BlockType::OakLog] = "Oak Log";
				BlockNameDatabase[BlockType::Cactus] = "Cactus";
				BlockNameDatabase[BlockType::Sand] = "Sand";
				BlockNameDatabase[BlockType::OakPlanks] = "Oak Planks";
				BlockNameDatabase[BlockType::AcaciaPlanks] = "Acacia Planks";
				BlockNameDatabase[BlockType::DarkOakPlanks] = "Dark Oak Planks";
				BlockNameDatabase[BlockType::Bricks] = "Bricks";
				BlockNameDatabase[BlockType::GlassWhite] = "Glass white";
				BlockNameDatabase[BlockType::Lamp_On] = "Lamp On";
				BlockNameDatabase[BlockType::Lamp_Off] = "Lamp Off";
				BlockNameDatabase[BlockType::WoolRed] = "Red Wool";
				BlockNameDatabase[BlockType::WoolGreen] = "Green Wool";
				BlockNameDatabase[BlockType::WoolBlue] = "Blue Wool";
				BlockNameDatabase[BlockType::WoolYellow] = "Yellow Wool";
				BlockNameDatabase[BlockType::Gravel] = "Gravel";
				BlockNameDatabase[BlockType::Clay] = "Clay";
				BlockNameDatabase[BlockType::Water] = "Water";
				BlockNameDatabase[BlockType::Lava] = "Lava";
				BlockNameDatabase[BlockType::Snow] = "Snow";
				BlockNameDatabase[BlockType::Slime] = "Slime";
				BlockNameDatabase[BlockType::Model_Grass] = "Grass [Model]";
				BlockNameDatabase[BlockType::Model_Deadbush] = "Deadbush [Model]";
				BlockNameDatabase[BlockType::Model_Kelp] = "Kelp [Model]";
				BlockNameDatabase[BlockType::Flower_allium] = "Flower (Allium) [Model]";
				BlockNameDatabase[BlockType::Flower_orchid] = "Flower (Orchid) [Model]";
				BlockNameDatabase[BlockType::Flower_tulip_red] = "Flower (Red Tulip) [Model]";
				BlockNameDatabase[BlockType::Flower_tulip_pink] = "Flower (Pink Tulip) [Model]";
				BlockNameDatabase[BlockType::Flower_rose] = "Flower (Rose) [Model]";
				BlockNameDatabase[BlockType::Flower_dandelion] = "Flower (Dandelion) [Model]";
				BlockNameDatabase[BlockType::UnknownBlockType] = "UNKNOWN BLOCK";
				BlockNameDatabase[BlockType::Bedrock] = "Bedrock";
				BlockNameDatabase[BlockType::Air] = "Air";
			}

			std::unordered_map<BlockType, std::string>::iterator iter = BlockNameDatabase.find(block_type);

			if (iter == BlockNameDatabase.end())
			{
				return BlockNameDatabase[BlockType::UnknownBlockType];
			}

			return BlockNameDatabase[block_type];
		}
	}
}