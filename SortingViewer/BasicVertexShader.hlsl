#include "Header.hlsli"
PSInput main(VSInput input) 
{
    PSInput output;
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, world);
    output.posWorld = pos;
    
    pos = mul(pos, view);
    pos = mul(pos, proj);
    output.posProj = pos;
    
    output.normal = input.normal;
    output.uv = input.uv;
    return output;
}