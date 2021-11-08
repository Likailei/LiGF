#include "Map.h"
#include "../src/Map.cpp"

int main() {
	Position p{ 1.0f, 2.0f, 3.0f };
	Map* map = new Map(p);

	//printf("%d\n", map->SpawnRegion.Chunks[0].data[0]);
	return 0;
}
