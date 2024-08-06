#include "Header.hlsli"
PSInput main(VSInput input) 
{
    PSInput output;
    output.posProj = float4(input.pos, 1.0f);
    output.posWorld = float4(input.pos, 1.0f);
    output.normal = input.normal;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}