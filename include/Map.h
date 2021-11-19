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
	Map(float seed);
	void GetAroundMesh(PlayerPos p, Mesh& m);
	
private:
	Noise* m_Noise;
	float m_Seed;
	std::vector<Region> m_Regions;

	void CreateChunkMesh(Region* region, IntPos chunkPos, Mesh& chunkMesh);

	inline void AddFront(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddBack(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddLeft(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddRight(Mesh& m, int x, int y, int z, UINT8 textureNum);
	inline void AddTop(Mesh& m, int x, int y, int z);
	inline void AddBottom(Mesh& m, int x, int y, int z);
};
}

#endif // !_MAP_H_