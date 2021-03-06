#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse; // ������������ �����
    sampler2D specular; // ���������� ��������� 
    float shininess; // ���� ������
}; 
// ������������ ��������
struct DirLight {
    vec3 direction; // ������������ ��������
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// �������� ��������
struct Light {
    vec3 position;  // ��������� ���������
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // ����������, �������� � ������������ ������������
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 outNormal;  
in vec2 TexCoords;
 

uniform Material material; 
uniform vec3 viewPos; // ��������� �����
uniform Light light;
uniform DirLight dirLight; 

vec3 IsDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 IsPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 norm = normalize(outNormal);
    vec3 viewDir = normalize(viewPos - FragPos); //  ������ ����������� ������� 

    vec3 result = IsDirLight(dirLight,norm,viewDir);
    
    result += IsPointLight(light, norm, FragPos, viewDir);
    
     FragColor = vec4(result, 1.0f);
}

vec3 IsDirLight(DirLight dirLight, vec3 normal, vec3 viewDir){
     
    vec3 lightDir = normalize(-dirLight.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);
   
    vec3 reflectDir = reflect(-lightDir, normal); //  ������ ��������� ������������ ���, ������� �������� �������
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  ��������� ��������� ����������� �����
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // ���������� ���������� ���������
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 IsPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos); // ������ ����������� �������� ��� ��������������� �� ���������
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal); //  ������ ��������� ������������ ���, ������� �������� �������
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //  ��������� ��������� ����������� �����
    
     //  �������� ���������� �� ��������� �� ��������� �����
    float distance    = length(light.position - fragPos);
    // ����������� ���������
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)); 
    // �������� ���������� ����������� �� ��� ���������� ������ ���������
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation; 
    
    return (ambient + diffuse + specular); // �������������� ���� ��������� ���������
   
}