#pragma once

#ifndef _STDAFX_H_
#define _STDAFX_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define SAFE_RELEASE(p) if (p) (p)->Release()

#include <Windows.h>
#include <windowsx.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3d12/d3dx12.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <wrl.h>

#include "Settings.h"

using namespace DirectX;

struct Vertex
{
    Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : position(x, y, z), uv(u, v), normal(nx,ny,nz) {}
    XMFLOAT3 position;
    XMFLOAT2 uv;
    XMFLOAT3 normal;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;
};

struct CharBufferInfo {
    wchar_t ch;
    unsigned int rows;
    unsigned int width;
    int pitch;
    signed long advanceX;
    signed long advanceY;
    signed long horiBearingY;
    unsigned char* buffer;
};

struct StringBufferInfo {
    const wchar_t* str;
    unsigned int rows;
    unsigned int width;
    int pitch;
    size_t size;
    unsigned char* buffer;
};

struct TextureInfo {
    UINT width;
    UINT height;
    UINT rowPitch;
    UINT8 pixelWidth;
    DXGI_FORMAT format;
    UINT* data;
};

struct ConstBufferObject {
    XMFLOAT4X4 mvpMat;
    float padding[48];
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        char str[64];
        sprintf_s(str, "HRESULT OF 0x%08X", static_cast<UINT>(hr));
        //OutputDebugStringA(str);
        throw std::runtime_error(str);
    }
}

inline float Clamp(float x, float low, float high) {
    return x < low ? low : (x > high ? high : x);
}

#endif // !_STDAFX_H_
