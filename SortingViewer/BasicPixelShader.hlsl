#include "Header.hlsli"
Texture2D g_texture : register(t0);
float4 main(PSInput input) : SV_TARGET
{
    float ratioHeight = input.posWorld.y / maxHeight;
    float ratioWidth = input.posWorld.x / maxWidth;
    float3 color = lerp(float3(0.0f, 0.0f, 1.0f), float3(1.0f, 0.3f, 0.0f), ratioHeight);
    if (ratioHeight > 0.7f)
        color = lerp(float3(0.0f, 0.7f, 0.3f), color, (1.0f - ratioHeight) / 0.3f);
    if (ratioHeight > 0.95f)
        color = lerp(float3(0.3f, 0.3f, 0.6f), color, (1.0f - ratioHeight) / 0.05f);
    if(stdElement)
        color = float3(1.0f, 0.0f, 0.0f);
    else if(cmpElement1)
        color = float3(0.0f, 0.0f, 1.0f);
    else if (cmpElement2)
        color = float3(0.0f, 1.0f, 0.0f);
    else if(findPos)
        color = lerp(0.0f, 0.4f, ratioWidth);
    
    return float4(color, 1.0f);
}