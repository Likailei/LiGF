#pragma once
#ifndef _REGION_H_
#define _REGION_H_

#include "Chunk.h"

namespace MyCraft
{
class Region {
public:
	static const INT RegionWidthByChunk = 32;
	static const INT RegionWidthByBlock = Chunk::ChunkWidthByBlock * RegionWidthByChunk;

	Region(IntPos worldPos) {
		NWCoordinate = CalcNorthWestCoord(worldPos);
		Chunks.resize(RegionWidthByChunk * RegionWidthByChunk);
	}

	IntPos GetPosition() const { return NWCoordinate; };

	std::vector<Chunk>& GetChunk() { return Chunks; }

	Chunk* GetChunk(IntPos worldPos) {
		int x = abs(worldPos.x - NWCoordinate.x);
		int y = abs(worldPos.y - NWCoordinate.y);
		return &Chunks[y * RegionWidthByChunk + x];
	};

private:
	IntPos NWCoordinate;	// Northwest
	std::vector<Chunk> Chunks;

	IntPos CalcNorthWestCoord(IntPos p) {
		IntPos ret{};
		if (p.x >= 0) {
			ret.x = p.x / RegionWidthByBlock * RegionWidthByBlock;
			if (p.y >= 0) ret.y = (p.y + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1);
			else ret.y = (-p.y) / RegionWidthByBlock * RegionWidthByBlock;
		}
		else {
			ret.x = -((-p.x + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1));
			if (p.y >= 0) ret.y = (p.y + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1);
			else ret.y = -((-p.y) / RegionWidthByBlock * RegionWidthByBlock);
		}
		return ret;
	}
};
}
#endif // !_REGION_H_
