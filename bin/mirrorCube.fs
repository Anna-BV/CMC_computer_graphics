#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec3 outPosition;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(outPosition - cameraPos);
    vec3 R = reflect(I, normalize(outNormal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}