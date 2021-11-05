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
    float3 normal : NORMAL;
    float4 position : SV_POSITION;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer constantObj : register(b0)
{
    float4x4 wvpMat;
};

PSInput VSMain(float3 normal : NORMAL, float3 position : POSITION)
{
    PSInput result;

    result.position = mul(position, wvpMat);
    result.normal = float4(normal, 1.0f);

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    
    //float4 gray = g_texture.Sample(g_sampler, input.uv);
    //float4 textColor = float4(0.4980f, 1.0000f, 0.6667f, 1.0f);

    //return textColor * gray.x;
    return float4(0.6f, 0.3f, 0.1f, 1.0f);
}
