#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "stdafx.h"

class Map
{
public:
	static void GeneratePerlinNoiseHMap(UINT8* data, UINT width, UINT height, double octave);

	static double m_Seed;
private:

	static double GetSeed() { return m_Seed; }
	static XMVECTOR Hash(const XMVECTOR& p);
	static XMVECTOR Fade(const XMVECTOR& p);
	static inline float Interp(float a, float b, float t);
	static float PerlinNoise(const XMVECTOR& p);
	static float Fbm(XMVECTOR& p);
};

#endif // !_MAP_H_