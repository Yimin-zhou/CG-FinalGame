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
    float scaledDistance = (distance - 0.5) * 0.01; // Offset and scale the distance
    scaledDistance = clamp(scaledDistance, 0.0, 1.0);

    // Phong shading 
    float lambert = max(dot(normalize(fragNormal), lightDirection), 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 ambient = 0.1 * lightColor;
    vec3 diffuse = 0.5 * lightColor * lambert;
    vec3 h = normalize(lightDirection + viewDirection);
    vec3 specular = 0.5 * lightColor * pow(max(dot(normalize(fragNormal), h), 0.0), 64);

    // Compute the brightness value
    float brightness = lambert + dot(specular, vec3(1.0, 1.0, 1.0));
    brightness = clamp(brightness, 0.0, 1.0);

    vec2 xToonTextureCoordinate = vec2(specular.r, scaledDistance);

    // Sample the toon texture with the distance-based factor
    vec3 toonColor = texture(toonMap, xToonTextureCoordinate).rgb;
    vec3 diffuseColor = texture(toonMap, vec2(lambert, scaledDistance)).rgb;

    // Combine the toon color with the ambient color
    vec3 ambientColor = vec3(0.1, 0.1, 0.1);
    fragColor = vec4(vec3(toonColor * 0.2 + diffuseColor * 0.8), 1.0);
}
