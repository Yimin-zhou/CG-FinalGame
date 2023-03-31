#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in mat4 instanceModelMatrix;
layout(location = 7) in vec4 instanceColor;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec4 particleColor;

uniform mat4 view;
uniform mat4 projection;


void main() 
{
    gl_Position = projection * view * instanceModelMatrix * vec4(position, 1);
    
    fragPosition = (instanceModelMatrix * vec4(position, 1)).xyz;
    fragNormal = mat3(instanceModelMatrix) * normal;
    fragTexCoord = texCoord;
    particleColor = instanceColor;
}
