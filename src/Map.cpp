#include "Map.h"

MyCraft::Map::Map(float seed) : m_Seed(seed)
{
	m_Noise = new Noise(seed);
}

void MyCraft::Map::GetAroundMesh(PlayerPos p, Mesh& m)
{
	// TODO: find if THE region exits.
	Region* r = new Region(p, m_Noise);

	IntPos centerPos = Region::PlayerToChunk(p);
	IntPos topLeftPos = IntPos{ centerPos.x - Chunk::ChunkWidthByBlock, 0, centerPos.z + Chunk::ChunkWidthByBlock };
	for (int z = 0; z < 3; z++) {
		for (int x = 0; x < 3; x++) {
			IntPos o{ topLeftPos.x + Chunk::ChunkWidthByBlock * x, 0, topLeftPos.z - Chunk::ChunkWidthByBlock * z };
			CreateChunkMesh(r, o, m);
		}
	}

}

void MyCraft::Map::CreateChunkMesh(Region* region, IntPos chunkPos, Mesh& chunkMesh)
{
	Chunk& chunk = region->GetChunkByWorldPos(chunkPos);
	auto regionPos = region->GetPosition();

	for (int z = 0; z < MyCraft::Chunk::ChunkWidthByBlock; z++) {
		for (int x = 0; x < MyCraft::Chunk::ChunkWidthByBlock; x++) {
			for (int y = 0; y < MyCraft::Chunk::ChunkHeightByBlock; y++) {
				if (chunk.GetBlockType(x, y, z) == MyCraft::BlockType::GRASS) {
					int blockX = chunkPos.x + x;
					int blockY = chunkPos.y + y;
					int blockZ = chunkPos.z - z;

					bool topisair = (chunk.GetBlockType(x, y + 1, z) == MyCraft::BlockType::AIR);
					//front face
					if (z == 15) {
						if (chunkPos.z != regionPos.z - 496) {
							int worldX = chunkPos.x + x;
							int worldZ = chunkPos.z - z - 1;
							if (region->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								topisair ? AddFront(chunkMesh, blockX, blockY, blockZ, 0) : AddFront(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x, y, z + 1) == MyCraft::BlockType::AIR) {
						topisair ? AddFront(chunkMesh, blockX, blockY, blockZ, 0) : AddFront(chunkMesh, blockX, blockY, blockZ, 1);
					}

					//right side face
					if (x == 15) {
						if (chunkPos.x != regionPos.x + 496) {
							int worldX = chunkPos.x + x + 1;
							int worldZ = chunkPos.z - z;
							if (region->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								topisair ? AddRight(chunkMesh, blockX, blockY, blockZ, 0) : AddRight(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x + 1, y, z) == MyCraft::BlockType::AIR) {
						topisair ? AddRight(chunkMesh, blockX, blockY, blockZ, 0) : AddRight(chunkMesh, blockX, blockY, blockZ, 1);
					}

					//left side face
					if (x == 0) {
						if (chunkPos.x != regionPos.x) {
							int worldX = chunkPos.x + x - 1;
							int worldZ = chunkPos.z - z;
							if (region->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								topisair ? AddLeft(chunkMesh, blockX, blockY, blockZ, 0) : AddLeft(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x - 1, y, z) == MyCraft::BlockType::AIR) {
						topisair ? AddLeft(chunkMesh, blockX, blockY, blockZ, 0) : AddLeft(chunkMesh, blockX, blockY, blockZ, 1);
					}

					//back face
					if (z == 0) {
						if (chunkPos.z != regionPos.z) {
							int worldX = chunkPos.x + x;
							int worldZ = chunkPos.z - z + 1;
							if (region->GetTypeByWorldPos(worldX, y, worldZ) == MyCraft::BlockType::AIR) {
								topisair ? AddBack(chunkMesh, blockX, blockY, blockZ, 0) : AddBack(chunkMesh, blockX, blockY, blockZ, 1);
							}
						}
					}
					else if (chunk.GetBlockType(x, y, z - 1) == MyCraft::BlockType::AIR) {
						topisair ? AddBack(chunkMesh, blockX, blockY, blockZ, 0) : AddBack(chunkMesh, blockX, blockY, blockZ, 1);
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

inline void MyCraft::Map::AddFront(Mesh& m, int x, int y, int z, UINT8 textureNum)
{
	UINT cnt = (UINT)m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.5f,  0.0f, 0.0f, 0.0f, -1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 1.0f,  0.5f, 0.0f, 0.0f, -1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f, 0.0f, 0.0f, -1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 1.0f,  0.0f, 0.0f, 0.0f, -1.0f });
	}
	else if (textureNum == 1) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.5f,  1.0f, 0.0f, 0.0f, -1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.0f,  0.5f, 0.0f, 0.0f, -1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f, 0.0f, 0.0f, -1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, -1.0f });
	}
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddBack(Mesh& m, int x, int y, int z, UINT8 textureNum)
{
	UINT cnt = (UINT)m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 1.0f,  0.5f, 0.0f, 0.0f, 1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 0.5f,  0.5f, 0.0f, 0.0f, 1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f });
	}
	else if (textureNum == 1) {
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 0.5f,  0.5f, 0.0f, 0.0f, 1.0f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f });
	}
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddLeft(Mesh& m, int x, int y, int z, UINT8 textureNum)
{
	UINT cnt = (UINT)m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 0.5f,   0.0f, -1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 1.0f,  0.5f, -1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 0.5f,   0.5f, -1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 0.0f });
	}
	else if (textureNum == 1) {
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f,  0.5f,  1.0f, -1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.0f,  0.5f, -1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.0f,  1.0f, -1.0f, 0.0f, 0.0f });
	}
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddRight(Mesh& m, int x, int y, int z, UINT8 textureNum)
{
	UINT cnt = (UINT)m.vertices.size();
	if (textureNum == 0) {
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f, 1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 1.0f,   0.0f, 1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 1.0f,   0.5f, 1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.5f,  0.0f, 1.0f, 0.0f, 0.0f });
	}
	else if (textureNum == 1) {
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.5f,  1.0f, 1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f,  0.0f,  0.5f, 1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f });
		m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f });
	}
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddTop(Mesh& m, int x, int y, int z)
{
	UINT cnt = (UINT)m.vertices.size();
	m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + -1.0f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f });
	m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + 0.0f, 0.5f,   0.0f, 0.0f, 1.0f, 0.0f });
	m.vertices.push_back({ x + 1.0f,  y + 1.0f, z + -1.0f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f });
	m.vertices.push_back({ x + 0.0f,  y + 1.0f, z + 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f });
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}

inline void MyCraft::Map::AddBottom(Mesh& m, int x, int y, int z)
{
	UINT cnt = (UINT)m.vertices.size();
	m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + 0.0f, 0.5f,   1.0f, 0.0f, -1.0f, 0.0f });
	m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + -1.0f, 0.0f,  0.5f, 0.0f, -1.0f, 0.0f });
	m.vertices.push_back({ x + 1.0f,  y + 0.0f, z + -1.0f, 0.5f,  0.5f, 0.0f, -1.0f, 0.0f });
	m.vertices.push_back({ x + 0.0f,  y + 0.0f, z + 0.0f, 0.0f,   1.0f, 0.0f, -1.0f, 0.0f });
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 1); m.indices.push_back(cnt + 2);
	m.indices.push_back(cnt + 0); m.indices.push_back(cnt + 3); m.indices.push_back(cnt + 1);
}
