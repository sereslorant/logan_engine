#version 330

struct lshMaterial
{
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

struct lshPointLight
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Attenuation;

    vec3 Position;

    bool Active;
};


uniform mat4 lr_ViewMatrix;

const int N_POINT_LIGHTS = 32;
uniform lshPointLight lr_PointLights[N_POINT_LIGHTS];

uniform bool lr_HasDiffuseMap;
uniform sampler2D lr_DiffuseMap;

uniform bool lr_HasNormalMap;
uniform sampler2D lr_NormalMap;

uniform bool lr_HasSpecularMap;
uniform sampler2D lr_SpecularMap;

uniform bool lr_HasEmissiveMap;
uniform sampler2D lr_EmissiveMap;

uniform lshMaterial lr_Material;

in vec3 lr_fPosition;

in vec3 lr_fNormal;
in vec3 lr_fTangent;
in vec3 lr_fBitangent;

in vec2 lr_fTexCoord;

layout(location = 0) out vec4 lr_FragColor;

vec3 lCameraPos = vec3(lr_ViewMatrix[3][0],lr_ViewMatrix[3][1],lr_ViewMatrix[3][2]);

vec4 lCalculatePointLight(lshPointLight point_light,vec4 tex_color,vec4 specular_color,vec3 normal)
{
    vec4 Ambient = vec4(point_light.Ambient,1.0) * vec4(lr_Material.Diffuse,1.0) * tex_color;

    vec3 SurfaceToLightVector = point_light.Position - lr_fPosition;
    vec3 nSurfaceToLightVector = normalize(SurfaceToLightVector);

    float dContribution = max(dot(normal,nSurfaceToLightVector),0.0);
    vec4 Diffuse = dContribution * vec4(point_light.Diffuse,1.0) * vec4(lr_Material.Diffuse,1.0) * tex_color;

    vec3 nSurfaceToCamera = normalize(lCameraPos - lr_fPosition);
    vec3 Reflection = -reflect(nSurfaceToLightVector,normal);

    float sContribution = pow(max(dot(nSurfaceToCamera,Reflection),0.0),lr_Material.Shininess);
    vec4 Specular = sContribution * vec4(point_light.Specular,1.0) * vec4(lr_Material.Specular,1.0) * specular_color;

    float Distance = length(SurfaceToLightVector);
    float Attenuation = 1.0/(point_light.Attenuation.x + point_light.Attenuation.y*Distance + point_light.Attenuation.z*Distance*Distance);

    return Attenuation*(Ambient+Diffuse+Specular);
}

void main()
{
    vec4 TexColor;
    if(lr_HasDiffuseMap)
	{
		TexColor = texture(lr_DiffuseMap,lr_fTexCoord);
	}
	else
	{
		TexColor = vec4(1.0,1.0,1.0,1.0);
	}

	vec4 SpecularColor;
    if(lr_HasSpecularMap)
	{
		SpecularColor = texture(lr_SpecularMap,lr_fTexCoord);
	}
	else
	{
		SpecularColor = vec4(1.0,1.0,1.0,1.0);
	}

	vec3 NormalVector;
	if(lr_HasNormalMap)
	{
		vec3 nNormal     = normalize(lr_fNormal);
		vec3 nTangent    = normalize(lr_fTangent);
		vec3 nBitangent  = normalize(lr_fBitangent);

		mat3 TangentMatrix = mat3(nTangent.x,nBitangent.x,nNormal.x,
								  nTangent.y,nBitangent.y,nNormal.y,
								  nTangent.z,nBitangent.z,nNormal.z);

		vec3 NormalCoordVector  = (texture(lr_NormalMap,lr_fTexCoord).rgb)*2.0 - 1.0;

		NormalVector = normalize(TangentMatrix * NormalCoordVector);
	}
	else
	{
		NormalVector = normalize(lr_fNormal);
	}

    vec4 Result = vec4(0.0,0.0,0.0,1.0);

    for(int i=0;i < N_POINT_LIGHTS;i++)
	{
		if(lr_PointLights[i].Active)
		{
			Result += lCalculatePointLight(lr_PointLights[i],TexColor,SpecularColor,NormalVector);
		}
	}

	if(lr_HasEmissiveMap)
	{
		vec4 EmissiveColor = texture(lr_EmissiveMap,lr_fTexCoord);
		EmissiveColor.a = 1.0;

		Result += EmissiveColor;
	}

	lr_FragColor = vec4(Result.rgb,1.0);
}
