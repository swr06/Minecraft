#include "WorldFileHandler.h"

namespace Minecraft
{
    namespace WorldFileHandler
    {
        struct PlayerData
        {
            FPSCamera camera;
            glm::vec3 position;
        };

        // takes in a string such as "-1, 2" and outputs an std::pair<int,int>
        std::pair<int, int> ParseChunkFilename(const std::string& str)
        {
            std::string x = str.substr(0, str.find(','));
            std::string y = str.substr(str.find(',') + 1);

            std::pair<int, int> ret_val;
            ret_val.first = std::stoi(x);
            ret_val.second = std::stoi(y);

            return ret_val;
        }

        bool SaveWorld(const std::string& world_name, World* world)
        {
            Timer timer("WORLD SAVE TIMER!");

            const string save_dir = "Saves/";
            stringstream cdata_dir_s; // chunk data directory
            stringstream dir_s;
            std::map<std::pair<int, int>, Chunk>* world_data = world->GetWorldData();

            dir_s << save_dir << world_name << "/";
            cdata_dir_s << save_dir << world_name << "/chunks/";

            // Delete the previous world data
            std::filesystem::remove_all(dir_s.str());

            // Create the new folder
            std::filesystem::create_directories(cdata_dir_s.str());

            // Write the chunks
            for (auto e = world_data->begin(); e != world_data->end(); e++)
            {
                ChunkFileHandler::WriteChunk(&e->second, cdata_dir_s.str());
            }

            // Writing the player data
            PlayerData player_data = { world->p_Player->p_Camera, world->p_Player->p_Position };

            // Open the player data file
            stringstream player_data_file_dir;
            
            player_data_file_dir << dir_s.str() << "player.bin";
            FILE* player_data_file = fopen(player_data_file_dir.str().c_str(), "w+");
            
            if (!player_data_file)
            {
                Logger::LogToConsole("WORLD SAVING ERROR!   |   UNABLE TO OPEN PLAYER DATA FILE!");
                return false;
            }
            
            fwrite((void*)&player_data, sizeof(PlayerData), 1, player_data_file);
            fclose(player_data_file);

            return true;
        }

        World* LoadWorld(const std::string& world_name)
        {
            World* world = new World;
            Player* player = new Player;
            stringstream cdata_dir_s; // chunk data directory
            stringstream player_file_dir;
            stringstream dir_s; // world directory

            const string save_dir = "Saves/";

            dir_s << save_dir << world_name << "/";
            cdata_dir_s << save_dir << world_name << "/chunks/";
            player_file_dir << dir_s.str() << "player.bin";
            
            // iterate through all the files in the chunk directory and read the binary data  
            for (const auto& entry : std::filesystem::directory_iterator(cdata_dir_s.str()))
            {
                std::pair<int, int> chunk_loc = ParseChunkFilename(entry.path().filename().string());
                Chunk* chunk = world->EmplaceChunkInMap(chunk_loc.first, chunk_loc.second);
                ChunkFileHandler::ReadChunk(chunk, entry.path().string());
            }

            // set the player data

            PlayerData player_data = { world->p_Player->p_Camera, world->p_Player->p_Position };

            return world;
        }
    }
}
