#pragma once
#ifndef _CRAFTHEAD_H
#define _CRAFTHEAD_H

#include <vector>
#include <Windows.h>
#include <DirectXMath.h>

typedef DirectX::XMINT3 IntPos;
typedef DirectX::XMFLOAT3 PlayerPos;

namespace MyCraft {
    // https://minecraft-ids.grahamedgecombe.com/
    enum class BlockType {
        AIR,
        STONE,
        GRASS,
        DIRT
    };

    const int SeaSurfaceHeight = 63;
}

#endif // !_CRAFTHEAD_H