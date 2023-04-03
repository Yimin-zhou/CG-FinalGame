#version 450

// Global variables for lighting calculations
layout(location = 1) uniform vec3 viewPos;
layout(location = 2) uniform vec3 lightPos;
layout(location = 3) uniform sampler2D toonMap;
//layout(location = 5) uniform float distanceOffset;
//layout(location = 6) uniform float distanceScale;

// Output for on-screen color
layout(location = 0) out vec4 outColor;

// Interpolated output data from vertex shader
in vec3 fragPos; // World-space position
in vec3 fragNormal; // World-space normal

void main()
{
    // Calculate the normal, light direction, and view direction
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Calculate the Lambertian diffuse term
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Calculate the Blinn-Phong specular term
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    float spec = 0;

    // Calculate brightness without Toon effects
    float brightness = diffuse + spec;

    // Calculate the distance between the viewer and the fragment
    float distance = length(viewPos - fragPos);
    float distanceOffset = 0;
    float distanceScale = 1;
    float scaledDistance = (distance + distanceOffset) * distanceScale;

    // Sample the texture using brightness and scaled distance
    vec4 toonColor = texture(toonMap, vec2(brightness, scaledDistance));

    // Output the final color
    outColor = vec4(toonColor.rgb, 1.0);
}
