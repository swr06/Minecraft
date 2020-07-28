#include "ChunkFileHandler.h"

namespace Minecraft
{
	namespace ChunkFileHandler
	{

		void ExtendString(std::string& str, int ex_amt, std::string& ex_c)
		{
			ex_amt = abs(static_cast<int>(ex_amt - str.size()));

			for (int i = 0; i < ex_amt; i++)
			{
				str.insert(0, ex_c);
			}
		}

		std::string GenerateFileName(const glm::vec2& position, const std::string& dir)
		{
			std::stringstream s;
			
			s << dir << position.x << "," << position.y;
			return s.str();
		}

		bool WriteChunk(Chunk* chunk, const std::string& dir)
		{
			FILE* outfile;
			std::stringstream s;
			std::string file_name = GenerateFileName(glm::vec2(chunk->p_Position.x, chunk->p_Position.y), dir);

			outfile = fopen(file_name.c_str(), "w+");

			if (outfile == NULL)
			{
				s << "Couldn't write chunk (" << file_name << ")";
				Logger::LogToConsole(s.str());

				return false;
			}

			for (int i = 0; i < CHUNK_SIZE_X; i++)
			{
				for (int j = 0; j < CHUNK_SIZE_Y; j++)
				{
					for (int k = 0; k < CHUNK_SIZE_Z; k++)
					{
						fwrite(&chunk->m_ChunkContents->at(i).at(j).at(k), sizeof(Block), 1, outfile);
					}
				}
			}

			fclose(outfile);
			return true;
		}

		bool ReadChunk(Chunk* chunk, const std::string& dir)
		{
			FILE* infile;
			std::stringstream s;
			std::string file_name = dir;

			infile = fopen(file_name.c_str(), "r");

			if (infile == NULL)
			{
				s << "Couldn't read chunk (" << file_name << ")";
				Logger::LogToConsole(s.str());

				return false;
			}

			for (int i = 0; i < CHUNK_SIZE_X; i++)
			{
				for (int j = 0; j < CHUNK_SIZE_Y; j++)
				{
					for (int k = 0; k < CHUNK_SIZE_Z; k++)
					{
						fread(&chunk->m_ChunkContents->at(i).at(j).at(k), sizeof(Block), 1, infile);
					}
				}
			}

			fclose(infile);
			return true;
		}
	}
}