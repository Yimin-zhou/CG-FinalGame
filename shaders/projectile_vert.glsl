#version 450

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;
layout(location = 3) uniform mat3 normalModelMatrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;

out vec4 temp;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1);
    temp = gl_Position;
    fragPosition = (model * vec4(position, 1)).xyz;
    fragNormal = normalModelMatrix * normal;
    fragTexCoord = texCoord;
}
