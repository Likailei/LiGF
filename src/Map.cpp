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
			/*IntPos p{};
			p.x = region->GetPosition().x + x * Chunk::ChunkWidthByBlock;
			p.z = region->GetPosition().z - z * Chunk::ChunkWidthByBlock;
			c.SetPosition(p);*/

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
	chunkPos.x = chunkPos.z = 0;
	Chunk& chunk = SpawnRegion->GetChunk(chunkPos);
	auto p = chunk.GetPosition();

	UINT iList[] = {
		//front face
		0, 1, 2,
		0, 3, 1,

		//left face
		4, 5, 6,
		4, 7, 5,

		//right face
		8, 9, 10,
		8, 11, 9,

		//back face
		12, 13, 14,
		12, 15, 13,

		//top face
		16, 17, 18,
		16, 19, 17,

		//bottom face
		20, 21, 22,
		20, 23, 21
	};

	UINT index = 0;
	for (int z = 0; z < MyCraft::Chunk::ChunkWidthByBlock; z++) {
		for (int x = 0; x < MyCraft::Chunk::ChunkWidthByBlock; x++) {
			for (int y = 0; y < MyCraft::Chunk::ChunkHeightByBlock; y++) {
				if (chunk.GetBlockType(IntPos(x, y, z)) == MyCraft::BlockType::GRASS) {
					IntPos blockCoord{};
					blockCoord.x = p.x + x;
					blockCoord.y = p.y + y;
					blockCoord.z = p.z - z;

					//front face
					if (z==15 || (z<15 && chunk.GetBlockType(IntPos(x, y, z + 1)) == MyCraft::BlockType::AIR)) {
						auto cnt = chunkMesh.vertices.size();
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 1.0f, blockCoord.z + -1.0f, 0.5f,  0.0f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 0.0f, blockCoord.z + -1.0f, 1.0f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 0.0f, blockCoord.z + -1.0f, 0.5f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 1.0f, blockCoord.z + -1.0f, 1.0f,  0.0f });
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 1); chunkMesh.indices.push_back(cnt + 2);
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 3); chunkMesh.indices.push_back(cnt + 1);
					}

					//right side face
					if (x==15 || (x<15 && chunk.GetBlockType(IntPos(x + 1, y, z)) == MyCraft::BlockType::AIR)) {
						auto cnt = chunkMesh.vertices.size();
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 0.0f, blockCoord.z + -1.0f, 0.5f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 1.0f, blockCoord.z + 0.0f, 1.0f,  0.0f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 0.0f, blockCoord.z + 0.0f, 1.0f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 1.0f, blockCoord.z + -1.0f, 0.5f,  0.0f });
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 1); chunkMesh.indices.push_back(cnt + 2);
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 3); chunkMesh.indices.push_back(cnt + 1);
					}

					//left side face
					if (x == 0 || ( x>0 && chunk.GetBlockType(IntPos(x - 1, y, z)) == MyCraft::BlockType::AIR)) {
						auto cnt = chunkMesh.vertices.size();
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 1.0f, blockCoord.z + 0.0f, 0.5f,  0.0f });
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 0.0f, blockCoord.z + -1.0f, 1.0f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 0.0f, blockCoord.z + 0.0f, 0.5f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 1.0f, blockCoord.z + -1.0f, 1.0f,  0.0f });
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 1); chunkMesh.indices.push_back(cnt + 2);
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 3); chunkMesh.indices.push_back(cnt + 1);
					}


					//back face
					if (z == 0 || (z > 0 && chunk.GetBlockType(IntPos(x, y, z - 1)) == MyCraft::BlockType::AIR)) {
						auto cnt = chunkMesh.vertices.size();
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 1.0f, blockCoord.z + 0.0f, 0.5f,  0.0f });
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 0.0f, blockCoord.z + 0.0f, 1.0f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 0.0f, blockCoord.z + 0.0f, 0.5f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 1.0f, blockCoord.z + 0.0f, 1.0f,  0.0f });
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 1); chunkMesh.indices.push_back(cnt + 2);
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 3); chunkMesh.indices.push_back(cnt + 1);
					}

					//top face
					if (chunk.GetBlockType(IntPos(x, y + 1, z)) == MyCraft::BlockType::AIR) {
						auto cnt = chunkMesh.vertices.size();
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 1.0f, blockCoord.z + -1.0f, 0.0f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 1.0f, blockCoord.z + 0.0f, 0.5f,  0.0f });
						chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 1.0f, blockCoord.z + -1.0f, 0.5f,  0.5f });
						chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 1.0f, blockCoord.z + 0.0f, 0.0f,  0.0f });
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 1); chunkMesh.indices.push_back(cnt + 2);
						chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 3); chunkMesh.indices.push_back(cnt + 1);
					}

					//bottom face
					if (y == 0 || (y>0 && chunk.GetBlockType(IntPos(x, y - 1, z)) == MyCraft::BlockType::AIR)) {
							auto cnt = chunkMesh.vertices.size();
							chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 0.0f, blockCoord.z + 0.0f, 0.5f,  1.0f });
							chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 0.0f, blockCoord.z + -1.0f, 0.0f,  0.5f });
							chunkMesh.vertices.push_back({ blockCoord.x + 1.0f,  blockCoord.y + 0.0f, blockCoord.z + -1.0f, 0.5f,  0.5f });
							chunkMesh.vertices.push_back({ blockCoord.x + 0.0f,  blockCoord.y + 0.0f, blockCoord.z + 0.0f, 0.0f,  1.0f });
							chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 1); chunkMesh.indices.push_back(cnt + 2);
							chunkMesh.indices.push_back(cnt + 0); chunkMesh.indices.push_back(cnt + 3); chunkMesh.indices.push_back(cnt + 1);
					}
				}
			}
		}
	}

}

void MyCraft::Map::GetHmapByWorldPosition(UINT8* hmap, IntPos worldPos)
{
	Noise::GenerateHMapFromPos(hmap, Region::RegionWidthByBlock, worldPos.x, worldPos.y);
}
