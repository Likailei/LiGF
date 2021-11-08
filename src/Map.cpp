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

	std::vector<Chunk>& Chunks = region->GetChunk();
	for (int y = 0; y < Region::RegionWidthByChunk; y++) {
		for (int x = 0; x < Region::RegionWidthByChunk; x++) {
			Chunk& c = Chunks[y * Region::RegionWidthByChunk + x];
			IntPos p{};			
			p.x = region->GetPosition().x + x * Chunk::ChunkWidthByBlock;
			p.y = region->GetPosition().y - y * Chunk::ChunkWidthByBlock;
			c.SetPosition(p);
			
			int pixelX = x * Chunk::ChunkWidthByBlock;
			int pixelY = y * Chunk::ChunkWidthByBlock;
			for (int j = 0; j < Chunk::ChunkWidthByBlock; j++) {
				for (int i = 0; i < Chunk::ChunkWidthByBlock; i++) {
					UINT8 h = hmap[(pixelY+j) * Region::RegionWidthByBlock + pixelX + i];
					auto ptr = c.GetPtrFromRelativePosition(i, j);
					for (int t = 0; t < h; t++) {
						*(ptr + t) = MyCraft::BlockType::GRASS;
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
