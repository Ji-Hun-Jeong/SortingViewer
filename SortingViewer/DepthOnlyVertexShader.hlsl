#include "Header.hlsli"
float4 main(VSInput input) : SV_Position
{
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, world);
    pos = mul(pos, light.lightView);
    pos = mul(pos, light.lightProj);
    return pos;
}