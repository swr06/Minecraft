#include "Chunk.h"

namespace Minecraft
{
	Chunk::Chunk(const glm::vec3 chunk_position) : p_Position(chunk_position), 
		p_MeshState(ChunkMeshState::Unbuilt), p_ChunkState(ChunkState::Ungenerated), p_LightMapState(ChunkLightMapState::UnmodifiedLightMap)
	{
		// Initialize all the blocks in the chunk to be air blocks

		memset(&p_ChunkContents, BlockType::Air, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
		memset(&p_ChunkLightInformation, 0, (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * 2);
	}

	Chunk::~Chunk()
	{
		
	}

	void Chunk::SetBlock(BlockType type, const glm::vec3& position)
	{
		Block b;
		b.p_BlockType = type;

		p_ChunkContents.at(position.x).at(position.y).at(position.z) = b;
	}

	int Chunk::GetSunlightAt(int x, int y, int z)
	{
		return p_ChunkLightInformation[x][y][z].x;
	}

	int Chunk::GetTorchLightAt(int x, int y, int z)
	{
		return p_ChunkLightInformation[x][y][z].y;
	}

	void Chunk::SetSunlightAt(int x, int y, int z, int light_val)
	{
		p_ChunkLightInformation[x][y][z].x = light_val;
		p_LightMapState = ChunkLightMapState::ModifiedLightMap;
	}

	void Chunk::SetTorchLightAt(int x, int y, int z, int light_val)
	{
		p_ChunkLightInformation[x][y][z].y = light_val;
		p_LightMapState = ChunkLightMapState::ModifiedLightMap;
	}

	void Chunk::Construct()
	{
		m_ChunkMesh.ConstructMesh(this, p_Position);
		p_MeshState = ChunkMeshState::Built;
	}

	ChunkMesh* Chunk::GetChunkMesh()
	{
		return &m_ChunkMesh;
	}

	Block* Chunk::GetBlock(int x, int y, int z)
	{
		return &p_ChunkContents[x][y][z];
	}
}