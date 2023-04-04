#version 450
layout(location = 0) uniform sampler2D blurTex;
layout(location = 1) uniform sampler2D screenTex;

layout(location = 0) in vec2 fragUV;

layout(location = 0) out vec4 fragColor;

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
    vec3 hdrColor = texture(screenTex, fragUV).rgb;      
    vec3 bloomColor = texture(blurTex, fragUV).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = aces(hdrColor);
    result = pow(result, vec3(1.0/2.2)); // gamma correction   
    fragColor = vec4(result, 1.0f);
}
