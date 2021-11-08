#pragma once

#ifndef _NOISE_H_
#define _NOISE_H_

#include "stdafx.h"

class Noise
{
public:
	static void GeneratePerlinNoiseHMap(UINT8* data, UINT width, UINT height, double octave);
	static void GenerateHMapFromPos(UINT8* data, UINT width, int x, int y);
	static double m_Seed;
private:

	static double GetSeed() { return m_Seed; }
	static XMVECTOR Hash(const XMVECTOR& p);
	static XMVECTOR Fade(const XMVECTOR& p);
	static inline float Interp(float a, float b, float t);
	static float PerlinNoise(const XMVECTOR& p);
	static float Fbm(XMVECTOR& p);
};

#endif