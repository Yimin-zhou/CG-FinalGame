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
uniform vec3 lightPos;

const float PI = 3.14159265359;

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

vec3 getWorldNormal()
{
    vec3 tangentNormal = texture(normalEmMap, fragTexCoord).rgb;
    tangentNormal = normalize(tangentNormal * 2.0 - 1.0);   ; // unpack

    // TODO tangent, bitangent
    return fragNormal;
}


void main()
{
    vec3 albedo     = pow(texture(albedoMap, fragTexCoord).rgb, vec3(2.2)) * baseColor; // srgb texture, remove gamma correction, cause ligth is cauculated in linear
    vec3 normal     = getWorldNormal();
    float roughness = texture(rmaMap, fragTexCoord).r * roughnessMultiplier;
    float metallic  = texture(rmaMap, fragTexCoord).g;
    float ao        = texture(rmaMap, fragTexCoord).b;
    vec3 emissive   = texture(normalEmMap, fragTexCoord).a * emissiveColor * 0;
    vec3 tangentNormal = texture(normalEmMap, fragTexCoord).rgb;

    vec3 N = normalize(fragNormal);
    vec3 V = normalize(cameraPos - fragPosition);

    vec3 F0 = vec3(0.04); // common value for non-metallic objects
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    vec3 L = normalize(lightPos - fragPosition);
    vec3 H = normalize(V + L);
    float distance    = length(lightPos - fragPosition);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = lightColor * attenuation * 500; // TODO
        
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
            
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    // indirect
    vec3 ambient = vec3(0.5) * albedo * ao; // TODO
    vec3 color = ambient + Lo + emissive;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); // gamma correction

    fragColor = vec4(color, 1.0);
}
