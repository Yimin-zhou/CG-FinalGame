#version 450
layout(location = 3) uniform sampler2D particalMap;
layout(location = 4) uniform vec4 particalColor;


in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
	vec4 color = texture(particalMap, fragTexCoord);
	fragColor = vec4(particalColor);
}
