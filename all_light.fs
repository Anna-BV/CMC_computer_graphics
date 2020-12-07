#version 330 core
out vec4 FragColor;

struct Material{
    sampler2D diffuse; // диффузионная карта
    sampler2D specular; // отражающий компонент
    float shininess; // сила блеска
};
// направленный источник освещения
struct DirectLight{
    vec3 direction; // направленние источник
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// точечные источники освещения
struct PointLight{
    vec3 position; // положение источника

    float constant; // постоянный, линейный и квадратичный коэффициенты
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// прожектор
struct SpotLight{
    vec3 position; // положение источника
    vec3 direction; // направленный источник
    float cutOff; // угол отсечения 
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // постоянный, линейный и квадратичный коэффициенты
    float linear;
    float quadratic;
};
// число точечных источников
#define NR_POINT_LIGHTS 2 

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material; //
uniform vec3 viewPos; // положение камер
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform DirectLight directLight; 

vec3 CalcDirLight(DirectLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos); //  вектор направления взгляда 

     vec3 result = CalcDirLight(directLight,norm,viewDir);

     for (int i = 0; i < NR_POINT_LIGHTS; i++){
          result += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
     }

     result += CalcSpotLight(spotLight, norm, FragPos, viewDir) ;

     FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirectLight light, vec3 normal, vec3 viewDir){
     
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);
   
    vec3 reflectDir = reflect(-lightDir, normal); //  вектор отражения относительно оси, которой является нормаль
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  вычисляем компонент зеркального блика
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // компонента диффузного освещения
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // определим, находимся мы внутри зоны действия прожектора или нет
    // скалярное произведение
    // инвертированный вектор direction (поскольку нам требуется вектор, направленный к источнику, а не от него).
    vec3 lightDir = normalize(light.position - fragPos);  // вектор направления задается как ориентированный от источника
    float diff = max(dot(normal, lightDir), 0.0);
      
   
    vec3 reflectDir = reflect(-lightDir, normal); //  вектор отражения относительно оси, которой является нормаль
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  вычисляем компонент зеркального блика
       
        //  значение расстояния от фрагмента до источника света
        float distance    = length(light.position - fragPos);
        // коэффициент затухания
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
        
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon   = light.cutOff - light.outerCutOff; // разница косинусов углов, внутр и внеш конуса
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);// функцию clamp(), которая ограничивает область значений своего первого параметра значениями второго и третьего параметров
         
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)); 
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); 
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

        // умножаем полученный коэффициент на все компоненты модели освещения
        ambient  *= attenuation * intensity;  
        diffuse   *= attenuation * intensity;
        specular *= attenuation * intensity; 
    
        return (ambient + diffuse + specular); // результирующий цвет выходного фрагмента
        
}