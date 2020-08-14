#include "BlockTextureManager.h"

namespace Minecraft
{
	/*
	A hacky way to map block faces to texture coordinates. :)
	Don't bother understanding it

	If it works, it works
	 ¯\_(ツ)_/¯
	*/

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
		Flower_allium,
		Flower_orchid,
		Flower_tulip_red,
		Flower_rose,
		Flower_houstonia,
		Flower_dandelion,
		UnknownBlockType,
		Air
	};

	BlockTypeTexture GetRealBlockTexture(BlockType block_type, BlockFaceType face_type)
	{
		uint8_t block = (BlockType)block_type;
		BlockTypeTexture ret_val = (BlockTypeTexture)block;

		switch (block_type)
		{
			case BlockType::Grass : 
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
		
			case BlockType::OakLog : 
			{
				if (face_type == BlockFaceType::top)
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

	const std::array<GLfloat, 8>& GetBlockTexture(BlockType block_type, BlockFaceType face_type)
	{
		static bool first_run = true;

		if (first_run)
		{
			GLClasses::TextureAtlas BlockTextureAtlas("Resources/BlockAtlasHighDef.png", 128, 128);
			first_run = false;

			constexpr int list_sz = 38;

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
				BlockTypeTexture::Flower_rose,
				BlockTypeTexture::Flower_dandelion,
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

		std::unordered_map<BlockTypeTexture, std::array<GLfloat, 8>>::iterator iter = BlockDatabase.find(GetRealBlockTexture(block_type, face_type));

		if (iter == BlockDatabase.end())
		{
			return BlockDatabase[BlockTypeTexture::UnknownBlockType];
		}

		return BlockDatabase[GetRealBlockTexture(block_type, face_type)];
	}
}