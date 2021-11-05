#include "Map.h"
#include <algorithm>
using namespace DirectX;

double Map::m_Seed = 43758.5453123;

void Map::GeneratePerlinNoiseHMap(UINT8* data, UINT width, UINT height, double octave)
{
	int index = 0;
	float step = .05f;

	float x = .0f;
	float y = .0f;
	for (int i = 0; i < height; i++)
	{
		y = i * .05f;
		for (int j = 0; j < width; j++)
		{
			x = j * .05f;
			XMVECTOR p = XMVectorSet(x, y, .0f, .0f);
			//float sum = PerlinNoise(p * octave);
			float sum = Fbm(p * octave);
			sum = (sum + 1.0f) * 255.f / 2.f;
			UINT8 c = (UINT8)floor(sum);

			data[index + 0] = c;
			data[index + 1] = c;
			data[index + 2] = c;
			data[index + 3] = 255;
			index += 4;
		}
	}
}

XMVECTOR Map::Hash(const XMVECTOR& p)
{
	XMVECTOR v = XMVectorSet(XMVectorGetX(XMVector2Dot(p, XMVectorSet(127.1f, 311.7f, .0f, .0f))),
							 XMVectorGetX(XMVector2Dot(p, XMVectorSet(269.5f, 183.3f, .0f, .0f))), .0f, .0f);
	v = XMVectorSin(v) * GetSeed();
	v = v - XMVectorFloor(v);
	return XMVectorSet(XMVectorGetX(v) * 2.f - 1.f, XMVectorGetY(v) * 2.f - 1.f, .0f, .0f);
}

XMVECTOR Map::Fade(const XMVECTOR& p)
{
	float x = XMVectorGetX(p);
	float y = XMVectorGetY(p);
	x = pow(x, 5.0f) * 6.f - pow(x, 4.0f) * 15.f + pow(x, 3.f) * 10;
	y = pow(y, 5.0f) * 6.f - pow(y, 4.0f) * 15.f + pow(y, 3.f) * 10;
	return XMVectorSet(x, y, .0f, .0f);
}

float Map::Interp(float a, float b, float t)
{
	return a + t * (b - a);
}

float Map::PerlinNoise(const XMVECTOR& p)
{
	XMVECTOR Pi = XMVectorFloor(p);
	XMVECTOR Pf = XMVectorSubtract(p, Pi);

	// tl----tr
	// | --p- |
	// | ---- |
	// bl----br
	XMVECTOR tl = XMVectorSet(0.0f, 0.0f, .0f, .0f);
	XMVECTOR tr = XMVectorSet(1.0f, 0.0f, .0f, .0f);
	XMVECTOR bl = XMVectorSet(0.0f, 1.0f, .0f, .0f);
	XMVECTOR br = XMVectorSet(1.0f, 1.0f, .0f, .0f);

	XMVECTOR tlp = XMVectorSubtract(Pf, tl);
	XMVECTOR trp = XMVectorSubtract(Pf, tr);
	XMVECTOR blp = XMVectorSubtract(Pf, bl);
	XMVECTOR brp = XMVectorSubtract(Pf, br);

	XMVECTOR tlN = XMVectorAdd(Pi, tl);
	XMVECTOR trN = XMVectorAdd(Pi, tr);
	XMVECTOR blN = XMVectorAdd(Pi, bl);
	XMVECTOR brN = XMVectorAdd(Pi, br);

	XMVECTOR tlGrad = XMVector2Normalize(Hash(tlN));
	XMVECTOR trGrad = XMVector2Normalize(Hash(trN));
	XMVECTOR blGrad = XMVector2Normalize(Hash(blN));
	XMVECTOR brGrad = XMVector2Normalize(Hash(brN));

	XMVECTOR t1 = XMVector2Dot(tlp, tlGrad);
	XMVECTOR t2 = XMVector2Dot(trp, trGrad);
	XMVECTOR t3 = XMVector2Dot(blp, blGrad);
	XMVECTOR t4 = XMVector2Dot(brp, brGrad);

	XMVECTOR f = Fade(Pf);

	return Interp(Interp(XMVectorGetX(t1), XMVectorGetX(t2), XMVectorGetX(f)),
				  Interp(XMVectorGetX(t3), XMVectorGetX(t4), XMVectorGetX(f)), XMVectorGetY(f));
}

float Map::Fbm(XMVECTOR& p)
{
	double f = 0.0;
	p = p * 4.0f;
	f += 1.0000f * PerlinNoise(p); p = p * 2.0f;
	f += 0.5000f * PerlinNoise(p); p = p * 2.0f;
	f += 0.2500f * PerlinNoise(p); p = p * 2.0f;
	f += 0.1250f * PerlinNoise(p); p = p * 2.0f;
	f += 0.0625f * PerlinNoise(p); p = p * 2.0f;

	return f;
}