#include "Region.h"

namespace MyCraft {

Region::Region(PlayerPos playerPos, Noise* noise) {
	Origin = PlayerToRegion(playerPos);

	for (int z = Origin.z; z > (Origin.z - RegionWidthByBlock); z -= Chunk::ChunkWidthByBlock) {
		for (int x = Origin.x; x < (Origin.x + RegionWidthByBlock); x += Chunk::ChunkWidthByBlock) {
			Chunks.push_back(Chunk(IntPos(x, 0, z)));
		}
	}

	InitRegionBlocks(noise);
}

IntPos Region::PlayerToChunk(PlayerPos& p) {
	IntPos o{};
	IntPos ip = IntPos{ (int)floor(p.x), 0, (int)floor(p.z) };
	o.x = ip.x >= 0 ? ip.x / Chunk::ChunkWidthByBlock * Chunk::ChunkWidthByBlock
		: (ip.x - Chunk::ChunkWidthByBlock) / Chunk::ChunkWidthByBlock * Chunk::ChunkWidthByBlock;
	o.z = ip.z > 0 ? (ip.z + Chunk::ChunkWidthByBlock) / Chunk::ChunkWidthByBlock * Chunk::ChunkWidthByBlock
		: ip.z / Chunk::ChunkWidthByBlock * Chunk::ChunkWidthByBlock;
	o.y = 0;
	return o;
}

IntPos Region::PlayerToRegion(PlayerPos& p) {
	IntPos o{};
	IntPos ip = IntPos{ (int)floor(p.x), (int)floor(p.y), (int)floor(p.z) };
	o.x = ip.x >= 0 ? ip.x / RegionWidthByBlock * RegionWidthByBlock
		: (ip.x - RegionWidthByBlock) / RegionWidthByBlock * RegionWidthByBlock;
	o.z = (ip.z + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1);
	o.y = 0;
	return o;
}

BlockType Region::GetTypeByWorldPos(int x, int y, int z) {
	auto c = GetChunkByWorldPos(IntPos(x, y, z));
	
	int t = x % Chunk::ChunkWidthByBlock;
	x = t < 0 ? t + Chunk::ChunkWidthByBlock : t;
	
	t = z % Chunk::ChunkWidthByBlock;
	z = t > 0 ? Chunk::ChunkWidthByBlock - t : -t;

	return c.GetBlockType(x, y, z);
}

inline Chunk& Region::GetChunkByWorldPos(IntPos worldPos) {
	auto p = W2L(worldPos);
	return Chunks[(p.z / Chunk::ChunkWidthByBlock) * RegionWidthByChunk + p.x / Chunk::ChunkWidthByBlock];
};

inline IntPos Region::W2L(const IntPos& world) {
	return IntPos{
		world.x - Origin.x,
		world.y,
		Origin.z - world.z,
	};
}

void Region::InitRegionBlocks(Noise* noise) {
	std::vector<UINT8> hmap;
	hmap.resize(RegionWidthByBlock * RegionWidthByBlock);
	noise->GenerateHMapFromPos(hmap.data(), RegionWidthByBlock, Origin.x, Origin.z);

	for (int z = 0; z < RegionWidthByChunk; z++) {
		for (int x = 0; x < RegionWidthByChunk; x++) {
			Chunk& c = Chunks.at(z * RegionWidthByChunk + x);
			int pixelX = x * Chunk::ChunkWidthByBlock;
			int pixelY = z * Chunk::ChunkWidthByBlock;
			for (int j = 0; j < Chunk::ChunkWidthByBlock; j++) {
				for (int i = 0; i < Chunk::ChunkWidthByBlock; i++) {
					UINT8 h = hmap[(pixelY + j) * RegionWidthByBlock + pixelX + i];
					auto ptr = c.GetColumnStartPtrByLocal(i, j);
					for (int t = 0; t < h; t++) {
						*(ptr + t) = MyCraft::BlockType::GRASS;
					}
				}
			}
		}
	}
}
}