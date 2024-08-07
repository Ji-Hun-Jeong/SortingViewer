struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 color : COLOR;
};

struct PSInput
{
    float4 posWorld : POSITION;
    float4 posProj : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 color : COLOR;
};

cbuffer GlobalConst : register(b0)
{
    matrix view;
    matrix proj;
};

SamplerState g_linearSampler : register(s0);
TextureCube g_specularCube : register(t10);
TextureCube g_irradianceCube : register(t11);
Texture2D g_brdfTexture : register(t12);