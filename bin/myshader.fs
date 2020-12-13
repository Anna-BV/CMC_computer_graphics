#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse; // диффузионная карта
    sampler2D specular; // отражающий компонент 
    float shininess; // сила блеска
}; 
// направленный источник
struct DirLight {
    vec3 direction; // направленние источник
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// точечный источник
struct Light {
    vec3 position;  // положение источника
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // постоянный, линейный и квадратичный коэффициенты
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 outNormal;  
in vec2 TexCoords;
 

uniform Material material; 
uniform vec3 viewPos; // положение камер
uniform Light light;
uniform DirLight dirLight; 

vec3 IsDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 IsPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 norm = normalize(outNormal);
    vec3 viewDir = normalize(viewPos - FragPos); //  вектор направления взгляда 

    vec3 result = IsDirLight(dirLight,norm,viewDir);
    
    result += IsPointLight(light, norm, FragPos, viewDir);
    
     FragColor = vec4(result, 1.0f);
}

vec3 IsDirLight(DirLight dirLight, vec3 normal, vec3 viewDir){
     
    vec3 lightDir = normalize(-dirLight.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);
   
    vec3 reflectDir = reflect(-lightDir, normal); //  вектор отражения относительно оси, которой является нормаль
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  вычисляем компонент зеркального блика
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // компонента диффузного освещения
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 IsPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos); // вектор направления задается как ориентированный от источника
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal); //  вектор отражения относительно оси, которой является нормаль
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  вычисляем компонент зеркального блика
    
     //  значение расстояния от фрагмента до источника света
    float distance    = length(light.position - fragPos);
    // коэффициент затухания
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)); 
    // умножаем полученный коэффициент на все компоненты модели освещения
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation; 
    
    return (ambient + diffuse + specular); // результирующий цвет выходного фрагмента
   
}