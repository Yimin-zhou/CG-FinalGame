#version 450

out vec4 FragColor;

in vec2 fragUV;

layout(location = 2) uniform sampler2D scene;
layout(location = 3) uniform sampler2D bloomBlur;
layout(location = 4) uniform float exposure;
layout(location = 5) uniform float bloomStrength = 0.04;


void main()
{
    vec3 hdrColor = texture(scene, fragUV).rgb;
    vec3 bloomColor = texture(bloomBlur, fragUV).rgb;
    vec3 result = mix(hdrColor, bloomColor, bloomStrength); // linear interpolation

    // tone mapping
    result = vec3(1.0) - exp(-result * exposure);
    // also gamma correct while we're at it
    const float gamma = 2.2;
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}