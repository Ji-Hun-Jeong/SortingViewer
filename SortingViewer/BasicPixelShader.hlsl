#include "Header.hlsli"
Texture2D g_texture : register(t0);
float4 main(PSInput input) : SV_TARGET
{
    float4 color = 1.0f;
    return g_texture.SampleLevel(g_linearSampler, input.uv, 1);
}