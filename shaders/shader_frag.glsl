#version 450
layout(location = 4) uniform sampler2D diffuseMap;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    const vec3 normal = normalize(fragNormal);

    fragColor = vec4(texture(diffuseMap, fragTexCoord).rgb * vec3((fragPosition + 1) * 0.3), 1);
}
