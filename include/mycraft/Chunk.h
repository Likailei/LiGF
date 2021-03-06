#pragma once
#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "CraftHead.h"

namespace MyCraft
{
class Chunk {
private:
	IntPos Origin;
	std::vector<BlockType> Blocks;

	IntPos WorldToLocal(const IntPos& worldPos){
		IntPos local{};

		local.x = worldPos.x<0 ? ChunkWidthByBlock + worldPos.x % ChunkWidthByBlock
							   : worldPos.x % ChunkWidthByBlock;
		local.z = worldPos.z>0 ? ChunkWidthByBlock - worldPos.z % ChunkWidthByBlock
							   : abs(worldPos.z) % ChunkWidthByBlock;
		local.y = worldPos.y;
		return local;
	}

public:
	static const INT ChunkWidthByBlock = 16;
	static const INT ChunkHeightByBlock = 256;
	static const INT ChunkBlockAmount = ChunkWidthByBlock * ChunkWidthByBlock * ChunkHeightByBlock;

	Chunk() {
		Blocks.resize(ChunkBlockAmount, BlockType::AIR);
	};

	Chunk(IntPos p) : Origin(p) {
		Blocks.resize(ChunkBlockAmount, BlockType::AIR);
	}
	IntPos GetPosition() const { return Origin; }
	void SetPosition(IntPos p) { Origin = p; }

	void SetBlockTypeByLocalPos(const IntPos& localPos, MyCraft::BlockType type){
		Blocks[localPos.z * ChunkWidthByBlock * ChunkHeightByBlock + localPos.x * ChunkHeightByBlock + localPos.y] = type;
	}

	void SetBlockTypeByWorldPos(const IntPos& worldPos, MyCraft::BlockType type){
		auto local = WorldToLocal(worldPos);
		SetBlockTypeByLocalPos(local, type);
	}

	BlockType* GetColumnStartPtrByLocal(int x, int z) {
		return Blocks.data() + z * ChunkWidthByBlock * ChunkHeightByBlock + x * ChunkHeightByBlock;
	}

	BlockType GetBlockType(int x, int y, int z) {
		return Blocks[z * ChunkWidthByBlock * ChunkHeightByBlock + x * ChunkHeightByBlock + y];
	}

};
}

#endif // !_CHUNK_H_