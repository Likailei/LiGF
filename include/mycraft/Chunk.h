#pragma once
#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "CraftHead.h"

namespace MyCraft
{
class Chunk {
private:
	IntPos Position;
	std::vector<BlockType> Blocks;

public:
	static const UINT ChunkWidthByBlock = 16;
	static const UINT ChunkHeightByBlock = 256;
	static const UINT ChunkBlockAmount = ChunkWidthByBlock * ChunkWidthByBlock * ChunkHeightByBlock;

	Chunk() {
		Blocks.resize(ChunkBlockAmount, BlockType::AIR);
	};

	Chunk(IntPos p) : Position(p) {
		Blocks.resize(ChunkBlockAmount, BlockType::AIR);
	}

	void SetPosition(IntPos p) { Position = p; }

	BlockType* GetPtrFromRelativePosition(int x, int y) {
		return Blocks.data() + y * ChunkWidthByBlock * ChunkHeightByBlock + x * ChunkHeightByBlock;
	}

	BlockType GetBlockType(IntPos p) {
		return Blocks[p.y * ChunkWidthByBlock * ChunkHeightByBlock + p.x * ChunkHeightByBlock + p.z];
	}
};
}

#endif // !_CHUNK_H_