#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TextureCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 outNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(Position, 1.0f));
    outNormal = mat3(transpose(inverse(model))) * Normal;  
    TexCoords = TextureCoords;
    
    gl_Position = projection * view * model * vec4(Position, 1.0f);
}