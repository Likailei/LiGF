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
			int sz = NWCoordinate.z;
			int sx = NWCoordinate.x;
			int index = 0;
			for (int z = sz; z > (sz - RegionWidthByBlock); z -= Chunk::ChunkWidthByBlock) {
				for (int x = sx; x < (sx + RegionWidthByBlock); x += Chunk::ChunkWidthByBlock) {
					Chunks[index].SetPosition(IntPos(x, 0, z));
					index++;
				}
			}
		}

		static IntPos PlayerPosToRegionOriginPos(const DirectX::XMFLOAT3& player) {
			IntPos regionPos{};
			IntPos p = IntPos{ (int)floor(player.x), (int)floor(player.y), (int)floor(player.z) };

			regionPos.x = p.x >= 0 ? p.x / RegionWidthByBlock * RegionWidthByBlock
				: -((-p.x + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1));
			regionPos.z = (p.z + RegionWidthByBlock - 1) & ~(RegionWidthByBlock - 1);
			regionPos.y = 0;
			return regionPos;
		}

		BlockType GetTypeByWorldPos(int x, int y, int z) {
			auto c = GetChunkByWorldPos(IntPos(x, y, z));
			x %= Chunk::ChunkWidthByBlock;
			z %= Chunk::ChunkWidthByBlock;
			x = x < 0 ? -x : x;
			z = z < 0 ? -z : z;
			return c.GetBlockType(x, y, z);
		}

		IntPos GetPosition() const { return NWCoordinate; };

		std::vector<Chunk>* GetChunks() { return &Chunks; }

		Chunk& GetChunkByWorldPos(IntPos worldPos) {
			auto p = W2L(worldPos);
			return Chunks[(p.z / Chunk::ChunkWidthByBlock) * RegionWidthByChunk + p.x / Chunk::ChunkWidthByBlock];
		};

	private:
		IntPos NWCoordinate;	// Northwest
		std::vector<Chunk> Chunks;

		inline IntPos W2L(const IntPos& world) {
			return IntPos{
				world.x - NWCoordinate.x,
				world.y,
				NWCoordinate.z - world.z,
			};
		}
	};
}
#endif // !_REGION_H_
