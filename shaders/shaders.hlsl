//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

static float3 g_sunPos = float3(5000.f, 5000.f, -1000.f);
static float g_ambientStrength = 0.1f;
static float3 g_sunColor = float3(1.f, 1.f, 1.f);

cbuffer constantObj : register(b0)
{
	float4x4 wvpMat;
};

PSInput VSMain(float4 position : POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL)
{
	PSInput result;

	result.position = mul(position, wvpMat);
	result.uv = uv;
	result.normal = normal;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float3 objColor = g_texture.Sample(g_sampler, input.uv);

	float3 ambient = g_ambientStrength * g_sunColor;

	float3 lightDir = g_sunPos - input.position.xyz;
	float d = max(dot(normalize(lightDir), input.normal), 0.f);
	float3 diffuse = d * g_sunColor;

	float3 color = objColor * (ambient + diffuse);

	return float4(color, 1.f);
}
