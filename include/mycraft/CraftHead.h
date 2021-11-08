#pragma once
#ifndef _CRAFTHEAD_H
#define _CRAFTHEAD_H

#include <vector>
#include <Windows.h>
#include <DirectXMath.h>

typedef DirectX::XMINT3 IntPos;

namespace MyCraft {
    // https://minecraft-ids.grahamedgecombe.com/
    enum class BlockType {
        AIR,
        STONE,
        GRASS,
        DIRT
    };
}

#endif // !_CRAFTHEAD_H