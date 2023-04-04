#version 450
layout(location = 0) uniform sampler2D screenTex;

layout(location = 0) in vec2 fragUV;

layout(location = 0) out vec4 fragColor;

void main()
{
	vec4 color = texture(screenTex, fragUV);
	fragColor = vec4(color.rgb, 1.0);
}
