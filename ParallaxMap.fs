#version 330 core
out vec4 FragColor;


 in   vec3 FragPos;
 in   vec2 TexCoords;
 in   vec3 TangentLightPos;
 in   vec3 TangentViewPos;
 in   vec3 TangentFragPos;


uniform sampler2D diffusePMap;
uniform sampler2D normalPMap;
uniform sampler2D depthPMap;

uniform float heightScale;


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    
    float layerDepth = 1.0 / numLayers; // размер слоя глубины 
    float currentLayerDepth = 0.0;

    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers; //  смещение текстурных координат

    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthPMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        
        currentTexCoords -= deltaTexCoords;

        currentDepthMapValue = texture(depthPMap, currentTexCoords).r;

        currentLayerDepth += layerDepth;
    }

	deltaTexCoords *= 0.5;
	layerDepth *= 0.5;
// сместимся в обратном направлении 
	currentTexCoords += deltaTexCoords;
	currentLayerDepth -= layerDepth;

	const int _reliefSteps = 5;
	int currentStep = _reliefSteps;
	while (currentStep > 0) {
		currentDepthMapValue = texture(depthPMap, currentTexCoords).r;
		deltaTexCoords *= 0.5;
		layerDepth *= 0.5;
// если выборка глубины больше текущей глубины слоя, 
//  в левую половину интервала
		if (currentDepthMapValue > currentLayerDepth) {
			currentTexCoords -= deltaTexCoords;
			currentLayerDepth += layerDepth;
		}
// иначе  в правую половину 
		else {
			currentTexCoords += deltaTexCoords;
			currentLayerDepth -= layerDepth;
		}
		currentStep--;
	}

	float lastDepthValue = currentDepthMapValue;
    
    return currentTexCoords;
}

void main()
{
    
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = TexCoords;
    
    texCoords = ParallaxMapping(TexCoords,  viewDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // obtain normal from normal map
    vec3 normal = texture(normalPMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
   
    // get diffuse color
    vec3 color = texture(diffusePMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

