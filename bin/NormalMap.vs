#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TextureCoords;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;


out  vec3 FragPos;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    FragPos = vec3(model * vec4(Position, 1.0));   
    TexCoords = TextureCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 T = normalize(normalMatrix * Tangent);
    vec3 N = normalize(normalMatrix * Normal);
    T = normalize(T - dot(T, N) * N);
    //  ������ �������� TBN ������� ���������������, ������������� ����� ����� ����� ���� ���������
    vec3 B = cross(N, T);
    // ���������� �������
    mat3 TBN = transpose(mat3(T, B, N));    
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
        
    gl_Position = projection * view * model * vec4(Position, 1.0);
}