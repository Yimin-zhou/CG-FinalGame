#version 450

layout(location = 4) uniform vec3 viewPosition;
layout(location = 5) uniform vec3 lightPosition;
layout(location = 6) uniform sampler2D albedoMap;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 color;

vec3 xToonShading(vec3 lightDirection, vec3 viewDirection, vec3 normal) {
    // Diffuse
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse = lightColor * diffuseFactor;

    // Specular
    vec3 halfwayVector = normalize(lightDirection + viewDirection);
    float shininess = 1;
    float specularFactor = pow(max(dot(normal, halfwayVector), 0.0), shininess);
    vec3 specular = lightColor * specularFactor;

    // X-Toon shading
    float xToonThreshold = 0.5;
    float xToonIntensity = smoothstep(xToonThreshold - 0.05, xToonThreshold + 0.05, diffuseFactor);

    return (diffuse + specular) * xToonIntensity;
}

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 texColor = texture(albedoMap, fragTexCoord).a * vec3(0.1, 0.8, 0.9);
    vec3 shadedColor = xToonShading(lightDirection, viewDirection, normal) * texColor;

    color = vec4(texColor, 1);
}
