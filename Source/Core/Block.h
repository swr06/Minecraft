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

		// Liquid or gas blocks
		Water, // To add
		Lava, // To add
		Snow, // To add
		Slime, // To add
		Air,

		UnknownBlockType
	};

	struct Block
	{
		std::uint8_t p_BlockType = BlockType::UnknownBlockType;

		bool IsOpaque()
		{
			if (p_BlockType != BlockType::Air && p_BlockType != BlockType::GlassWhite && p_BlockType != BlockType::Water)
			{
				return true;
			}

			return false;
		}
	};
}