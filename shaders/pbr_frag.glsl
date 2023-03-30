#version 450

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D albedoMap; // albedo, opacity, (srgb)
uniform sampler2D rmaMap; // roughness, metalness, ambient occlusion
uniform sampler2D normalEmMap; // normal, emissive

uniform float roughnessMultiplier;
uniform vec3 baseColor;
uniform vec3 lightColor;
uniform vec3 emissiveColor;

uniform vec3 cameraPos;

// Directional light struct
struct DirectionalLight 
{
    vec3 direction;
    vec3 color;
    float intensity;
};

// Point light struct
struct PointLight 
{
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

// Spot light struct
struct SpotLight 
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLights[4];

const float PI = 3.14159265359;

vec3 computeTangent()
{
    vec3 dp1 = dFdx(fragPosition);
    vec3 dp2 = dFdy(fragPosition);
    vec2 duv1 = dFdx(fragTexCoord);
    vec2 duv2 = dFdy(fragTexCoord);

    float r = 1.0 / (duv1.x * duv2.y - duv1.y * duv2.x);
    vec3 tangent = (dp1 * duv2.y - dp2 * duv1.y) * r;
    return normalize(tangent);
}

vec3 computeBitangent()
{
    vec3 dp1 = dFdx(fragPosition);
    vec3 dp2 = dFdy(fragPosition);
    vec2 duv1 = dFdx(fragTexCoord);
    vec2 duv2 = dFdy(fragTexCoord);

    float r = 1.0 / (duv1.x * duv2.y - duv1.y * duv2.x);
    vec3 bitangent = (dp2 * duv1.x - dp1 * duv2.x) * r;
    return normalize(bitangent);
}

vec3 getTBNNormal()
{
    vec3 texNormal = texture(normalEmMap, fragTexCoord).rgb;
    texNormal = normalize(texNormal * 2.0 - 1.0);   ; // unpack
    texNormal.xy *= 1.5;
    texNormal = normalize(texNormal);

    vec3 tangent = computeTangent();
    vec3 bitangent = computeBitangent();

    vec3 TBN_normal = normalize(mat3(tangent, bitangent, texNormal) * texNormal);
    
    return TBN_normal;
}

// learnopengl: https://learnopengl.com/PBR/Theory
float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 N, vec3 V, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    vec3 L = normalize(light.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = lightColor * light.intensity;
        
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    float NdotL = max(dot(N, L), 0.0);   
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 V, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    vec3 L = normalize(light.position - fragPosition);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 radiance = light.color * light.intensity;
        
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    float NdotL = max(dot(N, L), 0.0);   
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcSpotLight(SpotLight light, vec3 N, vec3 V, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    vec3 L = normalize(light.position - fragPosition);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(light.direction, normalize(-L)); // Angle between light direction and direction to fragment
    if (theta > light.outerCutoff) 
    {
        light.intensity = (theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff); // Smooth falloff between inner and outer cones
    } 
    else 
    {
        light.intensity = 0.0; // Outside the outer cone, no light
    }

    vec3 radiance = light.color * light.intensity;
        
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  

    float NdotL = max(dot(N, L), 0.0);   
    return (kD * albedo / PI + specular) * radiance * NdotL;
}


void main()
{
    vec3 albedo     = pow(texture(albedoMap, fragTexCoord).rgb, vec3(2.2)) * baseColor; // srgb texture, remove gamma correction, cause ligth is cauculated in linear
    vec3 normal     = getTBNNormal(); // TODO not used yet
    float roughness = texture(rmaMap, fragTexCoord).r * roughnessMultiplier;
    float metallic  = texture(rmaMap, fragTexCoord).g;
    float ao        = texture(rmaMap, fragTexCoord).b;
    vec3 emissive   = texture(normalEmMap, fragTexCoord).a * emissiveColor * 0;

    vec3 N = normalize(fragNormal);
    vec3 V = normalize(cameraPos - fragPosition);

    vec3 F0 = vec3(0.04); // common value for non-metallic objects
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // direct

    // directional
    Lo += CalcDirectionalLight(directionalLight, N, V, albedo, roughness, metallic, F0);

    // point
    for(int i = 0; i < 4; i++)
	{
		Lo += CalcPointLight(pointLights[i], N, V, albedo, roughness, metallic, F0);
	}

    // spot
    for(int i = 0; i < 4; i++)
    {
        Lo += CalcSpotLight(spotLights[i], N, V, albedo, roughness, metallic, F0);
    }

    // indirect
    vec3 ambient = vec3(0.5) * albedo * ao; // TODO
    vec3 color = ambient + Lo + emissive;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); // gamma correction

    fragColor = vec4(color, 1.0);
}
