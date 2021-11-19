#pragma once
#ifndef _REGION_H_
#define _REGION_H_

#include "Chunk.h"
#include "Noise.h"

namespace MyCraft
{
	class Region {
	public:
		static const INT RegionWidthByChunk = 32;
		static const INT RegionWidthByBlock = Chunk::ChunkWidthByBlock * RegionWidthByChunk;

		Region(PlayerPos playerPos, Noise* noise);
		static IntPos PlayerToChunk(PlayerPos& p);
		static IntPos PlayerToRegion(PlayerPos& p);
		BlockType GetTypeByWorldPos(int x, int y, int z);

		IntPos GetPosition() const { return Origin; };
		std::vector<Chunk>* GetChunks() { return &Chunks; }
		inline Chunk& GetChunkByWorldPos(IntPos worldPos);

	private:
		IntPos Origin;	// Northwest
		std::vector<Chunk> Chunks;

		inline IntPos W2L(const IntPos& world);
		void InitRegionBlocks(Noise* noise);
	};
}
#endif // !_REGION_H_
