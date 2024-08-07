#include "Header.hlsli"
cbuffer MeshConst : register(b1)
{
    matrix world;
}
PSInput main(VSInput input) 
{
    PSInput output;
    output.posWorld = float4(input.pos, 1.0f);
    output.posWorld = mul(output.posWorld, world);
    output.posProj = output.posWorld;
    output.normal = input.normal;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}