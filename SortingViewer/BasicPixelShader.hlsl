#include "Header.hlsli"
Texture2D g_texture : register(t0);
const static float PI = 3.141592f;
float3 LerpColorByHeight(float3 color, float ratioHeight)
{
    for (float standard = 0.0f; standard <= 1.0f; standard += 0.1f)
    {
        if (ratioHeight > standard)
            color = lerp(color, float3((1.0f - standard) * 0.5f, standard
            , 2.0f * standard), (ratioHeight - standard) / (1.0f - standard));
        else
            break;
    }
    return color;
}
float SpecularD(float ndoth, float roughness)
{
    float aa = roughness * roughness * roughness * roughness;
    float denominator = PI * pow(((ndoth * ndoth) * (aa - 1) + 1), 2.0f);
    return aa / denominator;
}
float Gx(float ndotx, float k)
{
    return ndotx / ((ndotx * (1 - k)) + k);
}
float SpecularG(float ndotl, float ndotv, float roughness)
{
    float k = pow((roughness + 1), 2.0f) / 8.0f;
    return Gx(ndotl, k) * Gx(ndotv, k);
}
float3 SpecularF(float3 F0, float vdoth)
{
    float square = (-5.55473 * vdoth - 6.98316) * vdoth;
    return F0 + (1.0f - F0) * pow(2.0f, square);
}
float3 AmbientLighting(float3 albedo, float3 F0, float3 n, float3 v, float3 h, float metallic
, float roughness)
{
    float3 diffuse = g_irradianceCube.SampleLevel(g_linearSampler, n, 0).rgb * (albedo - F0);
    float3 specular = g_specularCube.SampleLevel(g_linearSampler, reflect(-v, n), 0).rgb * F0;
    return (diffuse + specular);
}
float3 DirectLighting(float3 albedo, float3 F0, float3 n, float3 l, float3 v, float3 h
, float metallic, float roughness)
{
    float ndoth = max(0.0f, dot(n, h));
    float ndotl = max(0.0f, dot(n, l));
    float ndotv = max(0.0f, dot(n, v));
    float vdoth = max(0.0f, dot(v, h));
    float3 F = SpecularF(F0, vdoth);
    float D = SpecularD(ndoth, roughness);
    float G = SpecularG(ndotl, ndotv, roughness);
    
    float3 diffuse = albedo * (1.0f - F) / PI;
    float3 specular = D * F * G / max(1e-5, (4 * ndotl * ndotv));
    return (diffuse + specular) * light.strength;
}
float4 main(PSInput input) : SV_TARGET
{
    // 태양광이라고 가정
    float ratioWidth = input.posWorld.x / sumOfBoxWidth;
    float ratioHeight = input.posWorld.y / maxOfBoxHeight;
 
    float3 albedo = 1.0f;
    
    float3 n = normalize(input.normal);
    float3 l = normalize(light.pos - input.posWorld.xyz);
    float3 v = normalize(eyePos - input.posWorld.xyz);
    float3 h = normalize(l + v);
    float metallic = metalFactor;
    float roughness = roughFactor;
    
    float3 F0 = 0.04f;
    F0 = lerp(F0, albedo, metallic);
    
    float3 ambient = AmbientLighting(albedo, F0, n, v, h, metallic, roughness);
    float3 direct = DirectLighting(albedo, F0, n, l, v, h, metallic, roughness);
    albedo = ambient + direct;
    
    if (stdElement)
    {
        if (cmpElement2)
            albedo *= input.uv.x <= 0.5f ? float3(1.0f, 0.0f, 0.0f) : float3(0.0f, 1.0f, 0.0f);
        else
            albedo *= float3(1.0f, 0.0f, 0.0f);
    }
    else if (cmpElement1)
    {
        if (cmpElement2)
            albedo *= input.uv.x <= 0.5f ? float3(0.0f, 0.0f, 1.0f) : float3(0.0f, 1.0f, 0.0f);
        else
            albedo *= float3(0.0f, 0.0f, 1.0f);
    }
    else if (cmpElement2)
        albedo *= float3(0.0f, 1.0f, 0.0f);
    else if (findPos)
        albedo *= lerp(0.0f, 1.0f, ratioWidth);
    if (partition)
        albedo *= float3(1.0f, 0.0f, 1.0f);
    
    return float4(albedo, 1.0f);
}