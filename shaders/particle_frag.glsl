#version 450
layout(location = 3) uniform sampler2D particalMap;
layout(location = 4) uniform vec4 particalColor;


in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 bloomColor;

void main()
{
	vec4 color = texture(particalMap, fragTexCoord);
	fragColor = vec4(vec3(particalColor) * 5, particalColor.a);

	float brightness = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
    if(brightness > 0.8)
    {
        bloomColor = fragColor;
    }
    else
    {
        bloomColor = vec4(vec3(0), 1);
    }
}
