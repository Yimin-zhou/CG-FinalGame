#version 450

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform sampler2D toonMap;

out vec4 fragColor;

void main()
{
    // Compute the normalized direction vectors
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    vec3 viewDirection = normalize(viewPosition - fragPosition);

    // Compute the distance-based factor
    float distance = length(viewPosition - fragPosition);
    float distanceFactor = (distance - 0.5) * 0.5; // Offset and scale the distance
    distanceFactor = clamp(distanceFactor, 0, 1);

    // Compute the diffuse shading
    vec3 normal = normalize(fragNormal);
    float NdotL = max(dot(normal, lightDirection), 0.0);

    // Sample the toon texture with the distance-based factor
    vec3 toonColor = texture(toonMap, vec2(NdotL, distanceFactor)).rgb;

    // Combine the toon color with the ambient color
    vec3 ambientColor = vec3(0.1, 0.1, 0.1);
    fragColor = vec4(ambientColor + toonColor, 1.0);
}
