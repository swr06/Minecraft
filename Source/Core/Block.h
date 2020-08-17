#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Defs.h"

namespace Minecraft
{
	class Chunk;

	enum BlockType : std::uint8_t
	{
		Grass = 0,
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

		// Liquid or gas blocks
		Water, // To add
		Lava, // To add
		Snow, // To add
		Slime, // To add

		// Models
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

	struct Block
	{
		BlockType p_BlockType = BlockType::UnknownBlockType;

		bool IsModel() const
		{
			if (p_BlockType == Model_Grass ||
				p_BlockType == Model_Deadbush ||
				p_BlockType == Model_Kelp ||
				p_BlockType == Flower_allium ||
				p_BlockType == Flower_orchid ||
				p_BlockType == Flower_tulip_red ||
				p_BlockType == Flower_tulip_pink ||
				p_BlockType == Flower_rose ||
				p_BlockType == Flower_dandelion)
			{
				return true;
			}

			return false;
		}

		bool IsOpaque() const
		{
			if (p_BlockType != BlockType::Air && p_BlockType != BlockType::GlassWhite 
				&& p_BlockType != BlockType::Water && p_BlockType != BlockType::Cactus && p_BlockType != BlockType::OakLeaves 
				&& p_BlockType != BlockType::SpruceLeaves && !IsModel())
			{
				return true;
			}

			return false;
		}

		bool IsTransparent() const
		{
			if (p_BlockType == BlockType::Water || p_BlockType == BlockType::Lava 
				|| p_BlockType == BlockType::GlassWhite || p_BlockType == BlockType::Cactus || p_BlockType == BlockType::OakLeaves ||
				p_BlockType == BlockType::SpruceLeaves  ||  IsModel())
			{
				return true;
			}

			return false;
		}

		bool Collidable() const 
		{
			if (p_BlockType != BlockType::Air && p_BlockType != BlockType::Water && p_BlockType != BlockType::Lava && !IsModel())
			{
				return true;
			}

			return false;
		}

		bool IsLiquid() const 
		{
			if (p_BlockType == BlockType::Water || p_BlockType == BlockType::Lava)
			{
				return true;
			}

			return false;
		}
	};
}