#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse; // диффузионная карта
    sampler2D specular; // отражающий компонент
    float shininess; // сила блеска
}; 
struct Light {
    vec3 position; // положение источника

    //vec3 direction; // направленный источник

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // постоянный, линейный и квадратичный коэффициенты
    float linear;
    float quadratic;
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
   // компонента диффузного освещения
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); // вектор направления задается как ориентированный от источника
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // бликовое освещение
    vec3 viewDir = normalize(viewPos - FragPos); //  вектор направления взгляда 
    vec3 reflectDir = reflect(-lightDir, norm); //  вектор отражения относительно оси, которой является нормаль
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  вычисляем компонент зеркального блика
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb; 
    
     //  значение расстояния от фрагмента до источника света
    float distance    = length(light.position - FragPos);
    // коэффициент затухания
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
   
    // умножаем полученный коэффициент на все компоненты модели освещения
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation; 
    
    vec3 result = ambient + diffuse + specular; // результирующий цвет выходного фрагмента
    FragColor = vec4(result, 1.0f);
}