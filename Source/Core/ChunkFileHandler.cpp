#include "ChunkFileHandler.h"

namespace Minecraft
{
	namespace ChunkFileHandler
	{
		const std::string dir = "ChunkData/";

		void ExtendString(std::string& str, int ex_amt, std::string& ex_c)
		{
			ex_amt = abs(static_cast<int>(ex_amt - str.size()));

			for (int i = 0; i < ex_amt; i++)
			{
				str.insert(0, ex_c);
			}
		}

		std::string GenerateFileName(const glm::vec2& position)
		{
			std::stringstream s;
			
			s << dir << position.x << "," << position.y;
			return s.str();
		}

		bool UnloadChunk(Chunk* chunk)
		{
			FILE* outfile;
			std::stringstream s;
			std::string file_name = GenerateFileName(glm::vec2(chunk->p_Position.x, chunk->p_Position.y));

			outfile = fopen(file_name.c_str(), "w+");

			if (outfile == NULL)
			{
				s << "Couldn't write chunk (" << file_name << ")";
				Logger::LogToConsole(s.str());

				return false;
			}

			for (int i = 0; i < ChunkSizeX; i++)
			{
				for (int j = 0; j < ChunkSizeY; j++)
				{
					fwrite(&chunk->m_ChunkContents->at(i).at(j), sizeof(Block), ChunkSizeZ, outfile);
				}
			}

			fclose(outfile);
			return true;
		}

		bool LoadChunk(Chunk* chunk)
		{
			FILE* infile;
			std::stringstream s;
			std::string file_name = GenerateFileName(glm::vec2(chunk->p_Position.x, chunk->p_Position.y));

			infile = fopen(file_name.c_str(), "r");

			if (infile == NULL)
			{
				s << "Couldn't read chunk (" << file_name << ")";
				Logger::LogToConsole(s.str());

				return false;
			}

			for (int i = 0; i < ChunkSizeX; i++)
			{
				for (int j = 0; j < ChunkSizeY; j++)
				{
					fread(&chunk->m_ChunkContents->at(i).at(j), sizeof(Block), ChunkSizeZ, infile);
				}
			}

			fclose(infile);
			return true;
		}
	}
}