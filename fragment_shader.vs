#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse; // диффузионная карта
    vec3 specular; // бликовое освещение
    float shininess; // сила блеска
}; 
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material; //
uniform vec3 viewPos; // положение камер
uniform Light light;

void main()
{   // обновленные расчета
    // фоновое освещение
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); // вектор направления между источником света и фрагментом
    float diff = max(dot(norm, lightDir), 0.0);
    // компонента диффузного освещения
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    // бликовое освещение
    vec3 viewDir = normalize(viewPos - FragPos); //  вектор направления взгляда 
    vec3 reflectDir = reflect(-lightDir, norm); //  вектор отражения относительно оси, которой является нормаль
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  вычисляем компонент зеркального блика
    vec3 specular = (material.specular * spec) * light.specular;
    vec3 result = ambient + diffuse + specular; // результирующий цвет выходного фрагмента
    FragColor = vec4(result, 1.0f);
}