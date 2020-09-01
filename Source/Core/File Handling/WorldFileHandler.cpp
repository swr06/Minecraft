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

        struct WorldData
        {
            int seed;
            int world_gen_type;
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
            const std::map<std::pair<int,int>, Chunk>& world_data = world->GetWorldData();

            dir_s << save_dir << world_name << "/";
            cdata_dir_s << save_dir << world_name << "/chunks/";

            // If the directory already exists check if the two worlds is compatible 
            if (!std::filesystem::exists(dir_s.str()))
            {
                // Create the new folder
                std::filesystem::create_directories(cdata_dir_s.str());
            }

            else
            {
                std::stringstream existing_world_file_s;
                existing_world_file_s << dir_s.str() << "world.bin";

                FILE* existing_world_file = fopen(existing_world_file_s.str().c_str(), "rb");
                WorldData previous_save_data;

                fread(&previous_save_data, sizeof(WorldData), 1, existing_world_file);
                fclose(existing_world_file);

                if (previous_save_data.seed != world->GetSeed())
                {
                    Logger::LogToConsole("There is another incompatible world with the same name! World cannot be saved!");
                    return false;
                }
            }

            // Write the chunks
            for (auto e = world_data.begin() ; e != world_data.end() ; e++)
            {
                if (e->second.p_ChunkState == ChunkState::Changed ||
                    e->second.p_LightMapState == ChunkLightMapState::ModifiedLightMap)
                {
                    ChunkFileHandler::WriteChunk((Chunk*)&e->second, cdata_dir_s.str());
                }
            }

            // Writing the player data
            PlayerData player_data = { world->p_Player->p_Camera, world->p_Player->p_Position };

            // Open the player data file
            stringstream player_data_file_dir;
            
            player_data_file_dir << dir_s.str() << "player.bin";
            FILE* player_data_file = fopen(player_data_file_dir.str().c_str(), "wb+");
            
            if (!player_data_file)
            {
                Logger::LogToConsole("WORLD SAVING ERROR!   |   UNABLE TO OPEN PLAYER DATA FILE TO WRITE!");
                return false;
            }
            
            fwrite((void*)&player_data, sizeof(PlayerData), 1, player_data_file);
            fclose(player_data_file);

            // A file that has the seed of the world etc..
            FILE* world_data_file;
            WorldData world_data_w = { world->GetSeed(), world->GetWorldGenerationType() }; // the world data to write in the world.bin file
            stringstream world_data_file_pth;

            world_data_file_pth << dir_s.str() << "world.bin";
            world_data_file = fopen(world_data_file_pth.str().c_str(), "wb+");

            if (!world_data_file)
            {
                Logger::LogToConsole("WORLD SAVING ERROR!   |   UNABLE TO OPEN WORLD DATA FILE TO WRITE!");
                return false;
            }

            fwrite(&world_data_w, sizeof(WorldData), 1, world_data_file);
            fclose(world_data_file);

            return true;
        }

        World* LoadWorld(const std::string& world_name)
        {
            stringstream cdata_dir_s; // chunk data directory
            stringstream player_file_pth;
            stringstream world_file_pth;
            stringstream dir_s; // world directory

            const string save_dir = "Saves/";

            dir_s << save_dir << world_name << "/";
            cdata_dir_s << save_dir << world_name << "/chunks/";
            player_file_pth << dir_s.str() << "player.bin";
            world_file_pth << dir_s.str() << "world.bin";

            FILE* world_file = fopen(world_file_pth.str().c_str(), "rb");
            
            if (std::filesystem::is_directory(dir_s.str()) && std::filesystem::is_directory(cdata_dir_s.str()) && world_file_pth)
            {
                WorldData world_data;
                fread(&world_data, sizeof(WorldData), 1, world_file);
                fclose(world_file);

                World* world = new World(world_data.seed, {DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y}, world_name, (WorldGenerationType)world_data.world_gen_type);
               
                // iterate through all the files in the chunk directory and read the binary data  
                for (auto entry : std::filesystem::directory_iterator(cdata_dir_s.str()))
                {
                    std::pair<int, int> chunk_loc = ParseChunkFilename(entry.path().filename().string());
                    Chunk* chunk = world->EmplaceChunkInMap(chunk_loc.first, chunk_loc.second);
                    ChunkFileHandler::ReadChunk(chunk, entry.path().string());
                }

                // set the player data

                FILE* player_data_file;
                PlayerData player_data = { world->p_Player->p_Camera, world->p_Player->p_Position };

                if (std::filesystem::exists(player_file_pth.str()))
                {
                    player_data_file = fopen(player_file_pth.str().c_str(), "rb");
                    fread(&player_data, sizeof(PlayerData), 1, player_data_file);
                    fclose(player_data_file);
                }

                else
                {
                    stringstream s;
                    s << "Couldn't load player data from world dir ! PATH : " << player_file_pth.str() << "  IS INVALID!";

                    Logger::LogToConsole(s.str());
                }

                // Set the player data
                world->p_Player->p_Camera = player_data.camera;
                world->p_Player->p_Position = player_data.position;

                return world;
            }

            else
            {
                std::stringstream s;

                s << "WORLD LOADING FAILED! PATH : " << dir_s.str() << "  IS INVALID!";
                Logger::LogToConsole(s.str());

                return nullptr;
            }

        }
    }
}
