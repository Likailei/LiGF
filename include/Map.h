#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "mycraft/Region.h"
#include "Noise.h"

namespace MyCraft {
class Map
{
public:
	Map(IntPos p);
	void GenerateRegion(Region* region, IntPos worldPos);
	Region* SpawnRegion;
private:
	void GetHmapByWorldPosition(UINT8* hmap, IntPos worldPos);
};
}

#endif // !_MAP_H_