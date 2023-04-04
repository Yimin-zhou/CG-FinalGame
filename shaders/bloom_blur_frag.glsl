#version 450
layout(location = 2) uniform sampler2D screenTex;
layout(location = 3) uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 fragColor;

void main()
{
    vec2 tex_offset = 1.0 / textureSize(screenTex, 0); // gets size of single texel
    vec3 result = texture(screenTex, fragUV).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTex, fragUV + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(screenTex, fragUV - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTex, fragUV + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(screenTex, fragUV - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    fragColor = vec4(result, 1.0);
}
