#include "Map.h"

MyCraft::Map::Map(IntPos p)
{
	SpawnRegion = new Region(p);
	GenerateRegion(SpawnRegion, p);
}

void MyCraft::Map::GenerateRegion(Region* region, IntPos worldPos)
{
	std::vector<UINT8> hmap;
	hmap.resize(Region::RegionWidthByBlock * Region::RegionWidthByBlock);
	GetHmapByWorldPosition(hmap.data(), worldPos);

	auto Chunks = region->GetChunks();
	for (int z = 0; z < Region::RegionWidthByChunk; z++) {
		for (int x = 0; x < Region::RegionWidthByChunk; x++) {
			Chunk& c = Chunks->at(z * Region::RegionWidthByChunk + x);
			int pixelX = x * Chunk::ChunkWidthByBlock;
			int pixelY = z * Chunk::ChunkWidthByBlock;
			for (int j = 0; j < Chunk::ChunkWidthByBlock; j++) {
				for (int i = 0; i < Chunk::ChunkWidthByBlock; i++) {
					UINT8 h = hmap[(pixelY + j) * Region::RegionWidthByBlock + pixelX + i];
					auto ptr = c.GetColumnStartPtrByLocal(i, j);
					for (int t = 0; t < h; t++) {
						*(ptr + t) = MyCraft::BlockType::GRASS;
					}
				}
			}
		}
	}
}

void MyCraft::Map::CreateChunkMesh(IntPos chunkPos, Mesh& chunkMesh)
{
	// try to get the spawnregion's first chunk
	Chunk& chunk = SpawnRegion->GetChunkByWorldPos(chunkPos);
	auto regionPos = SpawnRegion->GetPosition();

	for (int z = 0; z < MyCraft::Chunk::ChunkWidthByBlock; z++) {
		for (int x = 0; x < MyCraft::Chunk::ChunkWidthByBlock; x++) {
			for (int y = 0; y < MyCraft::Chunk::ChunkHeightByBlock; y++) {
				if (chunk.GetBlockType(x, y, z) == MyCraft::BlockType::GRASS) {
					int blockX = chunkPos.x + x;
					int blockY = chunkPos.y + y;
					int blockZ = chunkPos.z - z;

					bool headisair = (chunk.GetBlockType(x, y + 1, z) == MyCraft::BlockType::AIR);
					//front face
					if (z == 15) {
						if (chunkPos.z != regionPos.z - 496) {
							int worldX = chunkPos.x + x;
							int worldZ = chunkPos.z - z - 1;
							if (SpawnRegion->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								headisair ? AddFront(chunkMesh, blockX, blockY, blockZ, 0) : AddFront(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x, y, z + 1) == MyCraft::BlockType::AIR) {
						headisair ? AddFront(chunkMesh, blockX, blockY, blockZ, 0) : AddFront(chunkMesh, blockX, blockY, blockZ, 1);
					}
					
					//right side face
					if (x == 15) {
						if (chunkPos.x != regionPos.x + 496) {
							int worldX = chunkPos.x + x + 1;
							int worldZ = chunkPos.z - z;
							if (SpawnRegion->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								headisair ? AddRight(chunkMesh, blockX, blockY, blockZ, 0) : AddRight(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x + 1, y, z) == MyCraft::BlockType::AIR) {
						headisair ? AddRight(chunkMesh, blockX, blockY, blockZ, 0) : AddRight(chunkMesh, blockX, blockY, blockZ, 1);
					}

					//left side face
					if (x == 0) {
						if (chunkPos.x != regionPos.x) {
							int worldX = chunkPos.x + x - 1;
							int worldZ = chunkPos.z - z;
							if (SpawnRegion->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								headisair ? AddLeft(chunkMesh, blockX, blockY, blockZ, 0) : AddLeft(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x - 1, y, z) == MyCraft::BlockType::AIR) {
						headisair ? AddLeft(chunkMesh, blockX, blockY, blockZ, 0) : AddLeft(chunkMesh, blockX, blockY, blockZ, 1);
					}

					//back face
					if (z == 0) {
						if (chunkPos.z != regionPos.z) {
							int worldX = chunkPos.x + x;
							int worldZ = chunkPos.z - z + 1;
							if (SpawnRegion->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								headisair ? AddBack(chunkMesh, blockX, blockY, blockZ, 0) : AddBack(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x, y, z - 1) == MyCraft::BlockType::AIR) {
						headisair ? AddBack(chunkMesh, blockX, blockY, blockZ, 0) : AddBack(chunkMesh, blockX, blockY, blockZ, 1);
					}

					////top face
					if (chunk.GetBlockType(x, y + 1, z) == MyCraft::BlockType::AIR) AddTop(chunkMesh, blockX, blockY, blockZ);

					//bottom face
					//if ((y > 0 && chunk.GetBlockType(x, y - 1, z) == MyCraft::BlockType::AIR)) goto BOTTOM;
				}
			}
		}
	}
}

void MyCraft::Map::GetHmapByWorldPosition(UINT8* hmap, IntPos worldPos)
{
	Noise::GenerateHMapFromPos(hmap, Region::RegionWidthByBlock, worldPos.x, worldPos.y);
}

inline void MyCraft::Map::AddFront(Mesh& m, float x, float y, float z, UINT8 textureNum)
{
	auto cnt = m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.5f,  0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 1.0f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 1.0f,  0.0f });
	}
	else if(textureNum == 1) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.5f,  1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.0f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.0f,  1.0f });
	}
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddBack(Mesh& m, float x, float y, float z, UINT8 textureNum)
{
	auto cnt = m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 0.5f,  0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 1.0f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 0.5f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 1.0f,  0.0f });
	}
	else if (textureNum == 1) {
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 0.5f,  1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 0.0f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 0.5f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 0.0f,  1.0f });
	}
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddLeft(Mesh& m, float x, float y, float z, UINT8 textureNum)
{
	auto cnt = m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 0.5f,  0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 1.0f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 0.5f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 1.0f,  0.0f });
	}
	else if (textureNum == 1) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f,  0.5f,  1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.0f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f,  0.5f,  0.5f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.0f,  1.0f });
	}
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddRight(Mesh& m, float x, float y, float z, UINT8 textureNum)
{
	auto cnt = m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 1.0f,  0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 1.0f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.5f,  0.0f });
	}
	else if (textureNum == 1) {
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.5f,  1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f,  0.0f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f,  0.5f,  0.5f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.0f,  1.0f });
	}	
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddTop(Mesh& m, float x, float y, float z)
{
	auto cnt = m.vertices.size();
	m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.0f,  0.5f });
	m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 0.5f,  0.0f });
	m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.5f,  0.5f });
	m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 0.0f,  0.0f });
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddBottom(Mesh& m, float x, float y, float z)
{
	auto cnt = m.vertices.size();
	m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 0.5f,  1.0f });
	m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.0f,  0.5f });
	m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f });
	m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 0.0f,  1.0f });
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}
