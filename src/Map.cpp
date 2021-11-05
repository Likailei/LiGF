#include "Map.h"

void Map::GeneratePerlinNoiseHMap(std::vector<UINT8>& const data, UINT width, UINT height, double seed)
{
	int index = 0;
	float step = 0.0f;
	float W = width * step;
	float H = height * step;
	for (float i = 0; i < H; i += step)
	{
		for (float j = 0; j < W; j += step)
		{
			XMVECTOR p = XMVectorSet(i, j, .0f, .0f);
			float sum = PerlinNoise(p);
			sum = sum * 255.0f / (2.0f * sqrtf(2.f));
			UINT8 c = (UINT8)floor(sum);
			data[index] = c;
			data[index + 1] = c;
			data[index + 2] = c;
			index += 3;
		}
	}
}

XMVECTOR Map::Hash(const XMVECTOR& p)
{
	XMVECTOR v = XMVectorSet(XMVectorGetX(XMVector2Dot(p, XMVectorSet(127.1f, 311.7f, .0f, .0f))),
							 XMVectorGetX(XMVector2Dot(p, XMVectorSet(269.5f, 183.3f, .0f, .0f))), .0f, .0f);
	v = XMVectorSin(v) * m_Seed;
	v = v - XMVectorFloor(v);
	return XMVectorSet(XMVectorGetX(v) * 2.f - 1.f, XMVectorGetY(v) * 2.f - 1.f, .0f, .0f);
}

XMVECTOR Map::Fade(const XMVECTOR& p)
{
	float x = XMVectorGetX(p);
	float y = XMVectorGetY(p);
	x = pow(x, 5.0f) * 6 - pow(x, 4.0f) * 15 + pow(x, 3) * 3;
	y = pow(y, 5.0f) * 6 - pow(y, 4.0f) * 15 + pow(y, 3) * 3;
	return XMVectorSet(x, y, .0, .0);
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

	XMVECTOR tlN = XMVectorAdd(Pf, tl);
	XMVECTOR trN = XMVectorAdd(Pf, tr);
	XMVECTOR blN = XMVectorAdd(Pf, bl);
	XMVECTOR brN = XMVectorAdd(Pf, br);

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
