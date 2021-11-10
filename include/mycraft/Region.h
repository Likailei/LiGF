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

		Region(IntPos regionPos) : NWCoordinate(regionPos) {
			Chunks.resize(RegionWidthByChunk * RegionWidthByChunk);
			for (int z = 0; z < RegionWidthByChunk; z++) {
				for (int x = 0; x < RegionWidthByChunk; x++) {
					Chunks[z * RegionWidthByChunk + x].SetPosition(IntPos(
						regionPos.x + x,
						0,
						regionPos.z - z
					));
				}
			}
		}

		static IntPos PlayerPosToRegionPos(const DirectX::XMFLOAT3& player) {
			IntPos regionPos{};
			IntPos p = IntPos{ (int)floor(player.x), (int)floor(player.y), (int)floor(player.z) };

			regionPos.x = p.x >= 0 ? p.x / RegionWidthByBlock * RegionWidthByBlock
				: -((-p.x + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1));
			regionPos.z = (p.z + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1);
			regionPos.y = 0;
			return regionPos;
		}

		IntPos GetPosition() const { return NWCoordinate; };

		std::vector<Chunk>* GetChunks() { return &Chunks; }

		Chunk& GetChunk(IntPos worldPos) {
			int x = worldPos.x - NWCoordinate.x;
			int z = NWCoordinate.z - worldPos.z;
			return Chunks[z * RegionWidthByChunk + x];
		};

	private:
		IntPos NWCoordinate;	// Northwest
		std::vector<Chunk> Chunks;
	};
}
#endif // !_REGION_H_
