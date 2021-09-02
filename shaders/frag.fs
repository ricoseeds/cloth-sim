//-----------------------------------------------------------------------------
// lighting_dir_point_spot.frag by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Fragment shader for multiple lights
//
// NOTE:
// This is not the most effient shader code but it gets the point across
// and should be easy to follow.  The same diffuse and specular calculations 
// are completed 3 separate times.  This can be optimized to be calculated
// only once with attenuation and spotlight multipliers applied.
//-----------------------------------------------------------------------------
#version 330 core

struct Material 
{
    vec3 ambient;
    sampler2D diffuseMap;
    vec3 specular;
    float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cosInnerCone;
	float cosOuterCone;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int on;

	float constant;
	float linear;
	float exponent;
};

  
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

#define MAX_POINT_LIGHTS 3

uniform DirectionalLight sunLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPos;

out vec4 frag_color;

vec3 calcDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLightColor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLightColor(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//-----------------------------------------------------------------------------------------------
// Main Shader Entry
//-----------------------------------------------------------------------------------------------
void main()
{ 
	vec3 normal = normalize(Normal);  
	vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient ----------------------------------------------------------------------------------
	vec3 ambient = spotLight.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));
	vec3 outColor = vec3(0.0f);	

	outColor += calcDirectionalLightColor(sunLight, normal, viewDir);

   for(int i = 0; i < MAX_POINT_LIGHTS; i++)
        outColor += calcPointLightColor(pointLights[i], normal, FragPos, viewDir);  

	// If the light isn't on then just return 0 for diffuse and specular colors
	//if (spotLight.on == 1)
	//	outColor += calcSpotLightColor(spotLight, normal, FragPos, viewDir);

	frag_color = vec4(ambient + outColor, 1.0f);
}

//-----------------------------------------------------------------------------------------------
// Calculate the direction light effect and return the resulting 
// diffuse and specular color summation
//-----------------------------------------------------------------------------------------------
vec3 calcDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);  // negate => Must be a direction from fragment towards the light

	// Diffuse ------------------------------------------------------------------------- --------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	return (diffuse + specular);
}

//-----------------------------------------------------------------------------------------------
// Calculate the point light effect and return the resulting diffuse and specular color summation
//-----------------------------------------------------------------------------------------------
vec3 calcPointLightColor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse ----------------------------------------------------------------------------------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	// Attenuation using Kc, Kl, Kq -------------------------------------------------------------
	float d = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

	diffuse *= attenuation;
	specular *= attenuation;
	
	return (diffuse + specular);
}

