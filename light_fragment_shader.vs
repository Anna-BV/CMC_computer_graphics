#version 330 core
out vec4 FragColor;

in vec3 Colorf;
void main()
{
    FragColor = vec4(Colorf,1.0f); // Устанавливает все 4 компоненты вектора равными 1.0f
}