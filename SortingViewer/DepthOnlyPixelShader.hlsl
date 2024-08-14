#include "Header.hlsli"
float4 main(float4 pos : SV_Position) : SV_TARGET
{
    float3 color = 1.0f;
    return float4(color, 1.0f);
}