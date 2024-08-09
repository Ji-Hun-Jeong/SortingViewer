struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSInput
{
    float4 posWorld : POSITION;
    float4 posProj : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer GlobalConst : register(b0)
{
    matrix view;
    matrix proj;
    matrix viewProj;
    float maxHeight;
    float3 eyePos;
    float maxWidth;
    float3 dummy;
};

cbuffer MeshConst : register(b1)
{
    matrix world;
    int stdElement;
    int cmpElement1;
    int cmpElement2;
    int findPos;
};

SamplerState g_linearSampler : register(s0);
TextureCube g_specularCube : register(t10);
TextureCube g_irradianceCube : register(t11);
Texture2D g_brdfTexture : register(t12);