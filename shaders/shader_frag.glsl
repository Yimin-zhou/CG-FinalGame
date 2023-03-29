#version 450
layout(location = 4) uniform sampler2D albedoMap;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    const vec3 normal = normalize(fragNormal);

    const vec3 lightDirection = normalize(vec3(0, 0, 1));

    // half lambert shading
    const float lambert = max(dot(normal, lightDirection), 0.0) * 0.8 + 0.3;
    const vec3 diffuse = texture(albedoMap, fragTexCoord).rgb * 3;
	fragColor = vec4(diffuse * lambert, 1.0);
}
