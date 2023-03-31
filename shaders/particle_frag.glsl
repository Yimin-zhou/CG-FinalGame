#version 450
layout(location = 4) uniform sampler2D particleMap;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    const vec3 emissiveColor = vec3(0.97, 0.21, 0.2);
	fragColor = vec4(emissiveColor, 1.0);
}
