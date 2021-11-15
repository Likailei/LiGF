#pragma once

#ifndef _NOISE_H_
#define _NOISE_H_

#include "stdafx.h"

class Noise
{
public:
	Noise() {};
	Noise(float seed);
	void GeneratePerlinNoiseHMap(UINT8* data, UINT width, UINT height, double octave);
	void GenerateHMapFromPos(UINT8* data, UINT width, int x, int y);

private:
	float m_Seed = 43758.545312f;
	XMVECTOR Hash(const XMVECTOR& p);
	XMVECTOR Fade(const XMVECTOR& p);
	inline float Interp(float a, float b, float t);
	float PerlinNoise(const XMVECTOR& p);
	float Fbm(XMVECTOR& p);
};

#endif