#include "Header.hlsli"
float3 ToneMapping(float3 color)
{
    color = pow(color, 1.0f / 1.0f);
    return color;
}
float4 main(PSInput input) : SV_TARGET
{
    float4 specular = g_specularCube.SampleLevel(g_linearSampler, input.normal, 0);
    float4 diffuse = g_irradianceCube.SampleLevel(g_linearSampler, input.normal, 0);
    return float4(ToneMapping(specular.rgb), 1.0f);
}