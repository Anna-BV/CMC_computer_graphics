#version 330 core
out vec4 FragColor;

in vec3 Colorf;
void main()
{
    FragColor = vec4(Colorf,1.0f); // ������������� ��� 4 ���������� ������� ������� 1.0f
}