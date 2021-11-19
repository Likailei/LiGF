#pragma once

#ifndef _NOISE_H_
#define _NOISE_H_

#include "stdafx.h"

class Noise
{
public:
	Noise(float seed) :m_Seed(seed) {};
	void GeneratePerlinNoiseHMap(UINT8* data, UINT width, UINT height, double octave);
	void GenerateHMapFromPos(UINT8* data, UINT width, int originX, int originY);

private:
	float m_Seed;
	XMVECTOR Hash(const XMVECTOR& p);
	XMVECTOR Fade(const XMVECTOR& p);
	float PerlinNoise(const XMVECTOR& p);
	float Fbm(XMVECTOR& p);

	inline float Interp(float a, float b, float t);
};

#endif