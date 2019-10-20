#ifndef DUCKY_LIGHTING_GLSL
#define DUCKY_LIGHTING_GLSL

#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

struct DirectionalLight
{
	vec3 direction;
	float intensity;
	vec3 color;
	float padding1;
};

struct PointLight
{
	vec3 color;
	float intensity;
	vec3 attenuations;
	float padding1;
	vec3 position;
	float padding2;
};

struct SpotLight
{
	vec3 color;
	float intensity;
	vec3 attenuations;
	float padding1;
	vec3 position;
	float padding2;
	vec3 direction;
	float cutoff;
};

uniform LightUniformBuffer
{
	DirectionalLight u_DirLights[4];
	PointLight u_PointLights[32];
	SpotLight u_SpotLights[64];
	int u_NumDirLights;
	int u_NumPointLights;
	int u_NumSpotLights;
	float u_Padding1;
	vec3 u_Ambient;
	float u_Padding2;
};

float saturate(float f)
{
	return clamp(f, 0, 1);
}

vec2 saturate(vec2 f)
{
	return clamp(f, 0, 1);	
}

vec3 saturate(vec3 f)
{
	return clamp(f, 0, 1);	
}

vec4 saturate(vec4 f)
{
	return clamp(f, 0, 1);	
}

vec4 GammaCorrectTexture(sampler2D s, vec2 uv)
{
	vec4 samp = texture(s, uv);
	return vec4(pow(samp.rgb, vec3(GAMMA)), samp.a);
}

vec3 GammaCorrectTextureRGB(sampler2D s, vec2 uv)
{
	vec4 samp = texture(s, uv);
	return vec3(pow(samp.rgb, vec3(GAMMA)));
}

vec4 GammaCorrectColor(vec4 v)
{
	return vec4(pow(v.rgb, vec3(GAMMA)), v.a);
}

vec3 GammaCorrectColorRGB(vec4 v)
{
	return vec3(pow(v.rgb, vec3(GAMMA)));
}

vec3 FinalGamma(vec3 color)
{
	return pow(color, vec3(1.0f / GAMMA));
}

vec3 NormalMap(vec3 normal, vec3 tangent, vec3 bitangent, sampler2D normalmap, vec2 uv)
{
	mat3 toWorld = mat3(tangent, bitangent, normal);
	vec3 normalMapColor = texture(normalmap, uv).rgb * 2.0 - 1.0;
	normalMapColor = toWorld * normalMapColor;
	return normalize(normalMapColor);
}

vec3 NormalMap(vec3 normal, vec3 tangent, vec3 bitangent, vec3 normalMapColor)
{
	mat3 toWorld = mat3(tangent, bitangent, normal);
	return normalize(toWorld * normalMapColor);
}

float FresnelSchlick(float f0, float fd90, float view)
{
	return f0 + (fd90 - f0) * pow(max(1.0f - view, 0.1f), 5.0f);
}

float Disney(vec3 normal, vec3 lightVector, float roughness, vec3 eye)
{
	vec3 halfVector = normalize(lightVector + eye);

	float NdotL = saturate(dot(normal, lightVector));
	float LdotH = saturate(dot(lightVector, halfVector));
	float NdotV = saturate(dot(normal, eye));

	float energyBias = mix(0.0f, 0.5f, roughness);
	float energyFactor = mix(1.0f, 1.0f / 1.51f, roughness);
	float fd90 = energyBias + 2.0f * (LdotH * LdotH) * roughness;
	float f0 = 1.0f;

	float lightScatter = FresnelSchlick(f0, fd90, NdotL);
	float viewScatter = FresnelSchlick(f0, fd90, NdotV);

	return lightScatter * viewScatter * energyFactor;
}

vec3 GGX(vec3 normal, vec3 lightVector, float roughness, vec3 specular, vec3 eye)
{
	vec3 h = normalize(lightVector + eye);
	float NdotH = saturate(dot(normal, h));

	float rough2 = max(roughness * roughness, 2.0e-3f); // capped so spec highlights don't disappear
	//float rough2 = roughness * roughness;
	float rough4 = rough2 * rough2;

	float d = (NdotH * rough4 - NdotH) * NdotH + 1.0f;
	float D = rough4 / (PI * (d * d));

	// Fresnel
	vec3 reflectivity = specular;
	float fresnel = 1.0;
	float NdotL = saturate(dot(normal, lightVector));
	float LdotH = saturate(dot(lightVector, h));
	float NdotV = saturate(dot(normal, eye));
	vec3 F = reflectivity + (fresnel - fresnel * reflectivity) * exp2((-5.55473f * LdotH - 6.98316f) * LdotH);

	// geometric / visibility
	float k = rough2 * 0.5f;
	float G_SmithL = NdotL * (1.0f - k) + k;
	float G_SmithV = NdotV * (1.0f - k) + k;
	float G = 0.25f / (G_SmithL * G_SmithV);

	return G * D * F;
}

float Blinn(vec3 normal, vec3 lightVector)
{
	return saturate(dot(normal, lightVector));
}

vec3 Phong(vec3 normal, vec3 lightVector, vec3 specular, vec3 eye)
{
	vec3 reflectionDirection = normalize(reflect(-lightVector, normal));
	float specularFactor = dot(eye, reflectionDirection);
	if(specularFactor > 0)
	{
		return specular * pow(specularFactor, 8);
	}
	return vec3(0, 0, 0);
}

float Diffuse(vec3 normal, vec3 lightVector, float roughness, vec3 eye)
{
	return Disney(normal, lightVector, roughness, eye);
	//return Blinn(normal, lightVector);
}

vec3 Specular(vec3 normal, vec3 lightVector, float roughness, vec3 specular, vec3 eye)
{
	return GGX(normal, lightVector, roughness, specular, eye);
	//return Phong(normal, lightVector, specular, eye);
}

float CalculateDirectionalLightAmount(vec3 lightDirection, vec3 normal)
{
	return saturate(dot(normal, lightDirection));
}

float CalculatePointLightAmount(vec3 attenuations, vec3 lightPosition, vec3 position, vec3 normal)
{
	float distanceToLight = length(lightPosition - position);
	float NdotL = saturate(dot(normal, normalize(lightPosition - position)));
	return NdotL * (1.0 / (attenuations.x +
		attenuations.y * distanceToLight +
		attenuations.z * distanceToLight * distanceToLight));
}

float CalculateSpotLightAmount(float cutoff, vec3 direction, vec3 attenuations, vec3 lightPosition, vec3 position, vec3 normal)
{
	vec3 lightToPixel = normalize(position - lightPosition);
    float spotFactor = dot(lightToPixel, direction);

    if (spotFactor > cutoff) {
        float distanceToLight = length(lightPosition - position);
		float NdotL = saturate(dot(normal, normalize(lightPosition - position)));
		float pointAmount = NdotL * (1.0 / (attenuations.x +
			attenuations.y * distanceToLight +
			attenuations.z * distanceToLight * distanceToLight));
        return (pointAmount * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - cutoff)));
    }
    return 0;
}

#endif
