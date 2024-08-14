#include "Header.hlsli"

RWTexture2D<float4> g_rtv : register(u0);
Texture2D g_shadowMap : register(t0);
[numthreads(32, 32, 1)]
void main( uint3 gtID : SV_GroupID ,uint3 dtID : SV_DispatchThreadID )
{
    float2 uv = dtID.xy / float2(1280, 960);
    float3 color = g_shadowMap.SampleLevel(g_linearSampler, uv, 0).rgb;
    g_rtv[dtID.rg] = float4(color, 1.0f);
}