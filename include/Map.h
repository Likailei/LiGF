#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "mycraft/Region.h"
#include "Noise.h"
#include "Timer.h"

namespace MyCraft {
class Map
{
public:
	Map(IntPos p);
	void GenerateRegion(Region* region, IntPos worldPos);
	void CreateChunkMesh(IntPos chunkPos, Mesh& chunkMesh);
	Region* SpawnRegion;
private:
	Noise* m_Noise;

	void GetHmapByWorldPosition(UINT8* hmap, IntPos worldPos);
	inline void AddFront(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddBack(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddLeft(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddRight(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddTop(Mesh& m, int x, int y, int z);
	inline void AddBottom(Mesh& m, int x, int y, int z);
};
}

#endif // !_MAP_H_