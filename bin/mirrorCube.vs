#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

out vec3 outNormal;
out vec3 outPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    outNormal = mat3(transpose(inverse(model))) * Normal;
    outPosition = vec3(model * vec4(Position, 1.0));
    gl_Position = projection * view * model * vec4(Position, 1.0);
}