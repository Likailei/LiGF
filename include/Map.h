#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "stdafx.h"

class Map
{
public:
	static void GeneratePerlinNoiseHMap(std::vector<UINT8>& const data, UINT width, UINT height, double seed);

private:
	static double m_Seed;

	static XMVECTOR Hash(const XMVECTOR& p);
	static XMVECTOR Fade(const XMVECTOR& p);
	static inline float Interp(float a, float b, float t);
	static float PerlinNoise(const XMVECTOR& p);
};

#endif // !_MAP_H_