#version 450

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalModelMatrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1);
    
    fragPosition = (model * vec4(position, 1)).xyz;
    fragNormal = normalModelMatrix * normal;
    fragTexCoord = texCoord;
}
