#include "Header.hlsli"
Texture2D g_texture : register(t0);
Texture2D g_shadowMap : register(t1);
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
float CalcAttenuation(float dist)
{
    return saturate((dist - light.fallOfStart) / (light.fallOfEnd - light.fallOfStart));
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
    float3 specular = g_specularCube.SampleLevel(g_linearSampler, normalize(reflect(-v, n)), 0).rgb;
    float2 brdf = g_brdfTexture.SampleLevel(g_linearSampler, float2(dot(v, n), 1 - roughness), 0).rg;
    specular *= brdf.r * F0 + brdf.g;
    return (diffuse + specular);
}
float2 ProjPosToUV(float3 projPos)
{
    float2 uv = projPos.xy;
    uv = uv * float2(1.0f, -1.0f);  // [-1, 1], [-1, 1]
    uv = uv + 1.0f;                 // [0, 2], [0, 2]
    uv /= 2.0f;
    return uv;
}
float GetShadowPower(float3 posWorld)
{
    float4 pos = float4(posWorld, 1.0f);
    pos = mul(pos, light.lightView);
    pos = mul(pos, light.lightProj);
    float3 projPosByLightView = pos.xyz / pos.w; // [-1, 1], [1, -1] -> [0, 1], [0, 1]
    if (projPosByLightView.x < -1 || projPosByLightView.x > 1 
        || projPosByLightView.y < -1 || projPosByLightView.y > 1)
        return 1.0f;
    float2 uv = ProjPosToUV(projPosByLightView);
    float minDepth = g_shadowMap.Sample(g_linearSampler, uv).r;
    float shadowPower = 1.0f;
    if (minDepth + 0.00001f < projPosByLightView.z)
        shadowPower = 0.0f;
    return shadowPower;
}

float3 DirectLighting(float3 posWorld, float3 albedo, float3 F0, float3 n, float3 l, float3 v, float3 h
, float metallic, float roughness)
{
    float3 pixelToLight = normalize(light.pos - posWorld);
    float ndoth = max(0.0f, dot(n, h));
    float ndotl = max(0.0f, dot(n, l));
    float ndotv = max(0.0f, dot(n, v));
    float vdoth = max(0.0f, dot(v, h));
    float3 F = SpecularF(F0, vdoth);
    float D = SpecularD(ndoth, roughness);
    float G = SpecularG(ndotl, ndotv, roughness);
    
    float3 diffuse = albedo * (1.0f - F) / PI;
    float3 specular = D * F * G / max(1e-5, (4 * ndotl * ndotv));
    float lightAndPixelTheta = max(0.0f, dot(light.lightDir, -pixelToLight));
    float dist = length(light.pos - posWorld);
    float3 lightStrength = light.strength * pow(lightAndPixelTheta * CalcAttenuation(dist), light.spotFactor);
    float shadowPower = GetShadowPower(posWorld);
    return (diffuse + specular) * lightStrength * ndotl * shadowPower;
}
float4 main(PSInput input) : SV_TARGET
{
    float ratioWidth = input.posWorld.x / sumOfBoxWidth;
    float ratioHeight = input.posWorld.y / maxOfBoxHeight;
 
    float3 albedo = g_texture.SampleLevel(g_linearSampler, input.uv, 0);
    albedo = albedo ? albedo : 1.0f;
    
    float3 n = normalize(input.normal);
    float3 l = normalize(light.pos - input.posWorld.xyz);
    float3 v = normalize(eyePos - input.posWorld.xyz);
    float3 h = normalize(l + v);
    float metallic = metalFactor;
    float roughness = roughFactor;
    
    float3 F0 = 0.04f;
    F0 = lerp(F0, albedo, metallic);
    
    float3 ambient = AmbientLighting(albedo, F0, n, v, h, metallic, roughness);
    float3 direct = DirectLighting(input.posWorld.xyz, albedo, F0, n, l, v, h, metallic, roughness);
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