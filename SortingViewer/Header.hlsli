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