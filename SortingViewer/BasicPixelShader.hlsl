#include "Header.hlsli"
float4 main(PSInput input) : SV_TARGET
{
    float3 color = input.color;
    return float4(color, 1.0f);
}