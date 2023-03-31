#version 450
layout(location = 4) uniform sampler2D particleMap;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 particleColor;

layout(location = 0) out vec4 fragColor;

void main()
{
    const vec3 emissiveColor = vec3(0.97, 0.21, 0.2);
	fragColor = vec4(vec3(particleColor), 1.0);
}
