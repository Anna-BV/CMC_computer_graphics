#include <GLAD/include/glad/glad.h>
#include  <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stb_image/stb_image.h>


#include "shader_h.h"
#include "camera_h.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void update_camera();
void mouse_moving(GLFWwindow* window, double xpos, double ypos);
void scroll_moving(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(std::vector<std::string> faces);
void renderQuad();

const unsigned int NWIDTH = 800, NHEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
bool firstMouse = true;

float lastTime = 0.0f; // Время вывода последнего кадра
float deltaTime = 0.0f;//  Время, прошедшее между последним и текущим кадром

float lastX = NWIDTH / 2.0;
float lastY = NHEIGHT / 2.0;

float heightScale = 0.1;


glm::vec3 lightPos(-3.0f, 1.0f, 8.5f);
int main(void)
{
    if (!glfwInit()) {
        std::cout << "ERROR::INITIALIZATION_GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window;
    // Открыть окно и создать в нем контекст OpenGL
    window = glfwCreateWindow(NWIDTH, NHEIGHT, "CMC_computer_graphics", NULL, NULL);
    if (!window)
    {
        std::cout << "ERROR::Failed_create _GLFWwindow" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_moving);
    glfwSetScrollCallback(window, scroll_moving);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, NWIDTH, NHEIGHT);
    // включаем тестирование трафаретов
    glEnable(GL_DEPTH_TEST);
    //определяет функцию, используемую для сравнения
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    // GL_NOTEQUAL	: Проходит, если входящее значение не равно установленному.
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // три параметра, с помощью которых мы можем определить действия для каждого варианта
    // GL_KEEP хранящееся в данный момент значение трафарета сохраняется
    // GL_REPLACE трафаретное значение заменяется эталонным значением, установленным функцией glStencilFunc
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // включим режим смешивания и выберем его параметры
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader lightShader("lightcub.vs", "lightcub.fs");
    Shader myShader("myshader.vs", "myshader.fs");
    Shader shaderColor("vshader.vs", "color.fs");
    Shader bilbordShader("vshader.vs", "bil.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");
    Shader mirrorCubeShader("mirrorCube.vs", "mirrorCube.fs");
    Shader normalMapShader("NormalMap.vs", "NormalMap.fs");
    Shader ParallaxMapShader("ParallaxMap.vs", "ParallaxMap.fs");
    // куб
    float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    // для куба с зеркальными гранями
    float mirrorCubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // пол
    float floorVertices[] = {
         5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f,  0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,

         5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,
         5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   1.0f,  1.0f
    };
    // билборд
    float bilbordsVertices[] = {
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
   // skybox
    float skyboxVertices[] = {          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
 
    unsigned int  ourcubeVBO, ourcubeVAO;
    glGenVertexArrays(1, &ourcubeVAO);
    glGenBuffers(1, &ourcubeVBO);

    glBindVertexArray(ourcubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ourcubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

   
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // лампа
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ourcubeVBO);
    // настройка атрибутов
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    unsigned int bilbordVAO, bilbordVBO;
    glGenVertexArrays(1, &bilbordVAO);
    glGenBuffers(1, &bilbordVBO);
    glBindVertexArray(bilbordVAO);

    glBindBuffer(GL_ARRAY_BUFFER, bilbordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bilbordsVertices), &bilbordsVertices, GL_STATIC_DRAW);
 
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int mirrorCubeVAO, mirrorCubeVBO;
    glGenVertexArrays(1, &mirrorCubeVAO);
    glGenBuffers(1, &mirrorCubeVBO);
    glBindVertexArray(mirrorCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mirrorCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mirrorCubeVertices), &mirrorCubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // текстуры для нашего skybox
    std::vector<std::string> skyboxTextures
    {
        "posx.jpg", // правая грань куба
        "negx.jpg", // левая грань куба
        "posy.jpg", // верхняя грань куба
        "negy.jpg", // нижняя грань куба
        "posz.jpg", // передняя грань куба
        "negz.jpg"  // задняя грань куба
    };
    // загрузка текстур
    // skybox
    unsigned int cubemapTexture = loadCubemap(skyboxTextures);
    // куб
    unsigned int diffuseMap = loadTexture("serebro.jpg");
    unsigned int specularMap = loadTexture("specular_serebro.jpg");
    // пол
    unsigned int diffFloorTexture = loadTexture("earth.jpg");
    unsigned int specFloorTexture = loadTexture("specular_earth.jpg");
    // билборд
    unsigned int bilbordTexture = loadTexture("pam.png");
    // NormalMap
    unsigned int diffuseNMap = loadTexture("Bricks.jpg");
    unsigned int normalNMap = loadTexture("NormalBricks.jpg"); 

    //ParallaxMp
    unsigned int diffusePMap = loadTexture("gold.jpg");
    unsigned int normalPMap = loadTexture("ornate-celtic-gold-normal-dx.png");
    unsigned int heightPMap = loadTexture("ornate-celtic-gold-height.png");

    myShader.Use();
    myShader.setInt("material.diffuse", 0);
    myShader.setInt("material.specular", 1);

    normalMapShader.Use();
    normalMapShader.setInt("diffuseNMap", 0);
    normalMapShader.setInt("normalNMap", 1);

    ParallaxMapShader.Use();
    ParallaxMapShader.setInt("diffusePMap", 0);
    ParallaxMapShader.setInt("normalPMap", 1);
    ParallaxMapShader.setInt("depthPMap", 2);


    mirrorCubeShader.Use();
    mirrorCubeShader.setInt("skybox", 0);

    skyboxShader.Use();
    skyboxShader.setInt("skybox", 0);

    bilbordShader.Use();
    bilbordShader.setInt("texture1", 0);

    std::vector<glm::vec3> bilbords
    {
            glm::vec3(-1.0f, 0.0f, -0.3f),
            glm::vec3(0.0f, 0.0f, 0.4f),
            glm::vec3(-0.2f, 0.0f, -2.3f),
            glm::vec3(0.5f, 0.0f, -0.6f)
    };
    
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;

        processInput(window);
        glfwPollEvents();
        update_camera();

        // для корректной работы смешивания при рендере начинаем объектов вывод с дальнего
        // упорядочивание на основе дистанции от объекта до наблюдателя
        std::map<float, glm::vec3> sortedBilbords;
        for (unsigned int i = 0; i < bilbords.size(); i++)
        {
            float distance = glm::length(camera.Position - bilbords[i]);
            sortedBilbords[distance] = bilbords[i];
        }
       
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
       
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

//...............эффект....NormalMap............................................................................
        projection = glm::perspective(camera.Zoom, (float)NWIDTH / (float)NHEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        normalMapShader.Use();
        normalMapShader.setMat4("projection", projection);
        normalMapShader.setMat4("view", view);
        // рендеринг normalMap четырехугольника
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 5.0f));
        model = glm::rotate(model, (float)glfwGetTime() * -10.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        normalMapShader.setMat4("model", model);
        normalMapShader.setVec3("viewPos", camera.Position);
        normalMapShader.setVec3("lightPos", lightPos);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseNMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalNMap);
        renderQuad();

        // рендеринг источника света 
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        normalMapShader.setMat4("model", model);
        renderQuad(); 

 //.............эффект......ParallexMap..................................................................   
        projection = glm::perspective(camera.Zoom, (float)NWIDTH / (float)NHEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        ParallaxMapShader.Use();
        ParallaxMapShader.setMat4("projection", projection);
        ParallaxMapShader.setMat4("view", view);
        // рендеринг ParallexMap четырехугольника
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 0.6f, 5.0f));
        ParallaxMapShader.setMat4("model", model);
        ParallaxMapShader.setVec3("viewPos", camera.Position);
        ParallaxMapShader.setVec3("lightPos", lightPos);
        ParallaxMapShader.setFloat("heightScale", heightScale); 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffusePMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalPMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, heightPMap);
        renderQuad();

        // рендеринг источника света 
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        ParallaxMapShader.setMat4("model", model);
        renderQuad();

        shaderColor.Use();
        projection = glm::perspective(camera.Zoom, (float)NWIDTH / (float)NHEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        shaderColor.setMat4("view", view);
        shaderColor.setMat4("projection", projection);
        model = glm::mat4(1.0f);

        myShader.Use();
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);
        
        // для освещения
        myShader.setVec3("viewPos", camera.Position);
        myShader.setFloat("material.shininess", 32.0f);
        // точечный источник
        myShader.setVec3("light.position", lightPos);
        myShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        myShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        myShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("light.constant", 1.0f);
        myShader.setFloat("light.linear", 0.09f);
        myShader.setFloat("light.quadratic", 0.032f);
        //направленный источник
        myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        myShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        myShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
 //........................ЭФФЕКТ ОБВОДКИ........................................................................
        // проверяем, что буфер трафарета не обновляется во время рисования
        glStencilMask(0x00);
        // рисуем пол
        glBindVertexArray(floorVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffFloorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specFloorTexture);
        myShader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // 1 этап рендеринга: рисуем сами кубики 
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // все фрагменты должны пройти тест трафарета
        glStencilMask(0xFF); // включаем запись в буфер трафарета
        glBindVertexArray(ourcubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        myShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        myShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // 2 подход рендеринга: увеличим кубики,отключив запись по трафарету
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // чтобы убедиться, что визуализируем только те конт, которые не = 1
        glStencilMask(0x00); // отключаем запись в буфер трафарета
        glDisable(GL_DEPTH_TEST);
        shaderColor.Use();
        float scale = 1.1;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        
        // источник света
        lightShader.Use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.4f)); // a smaller cube
        lightShader.setMat4("model", model);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

//.............зеркальные......грани....................................................................................
       
        mirrorCubeShader.Use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 3.0f, 0.0f));
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (float)NWIDTH / (float)NHEIGHT, 0.1f, 100.0f);
        mirrorCubeShader.setMat4("model", model);
        mirrorCubeShader.setMat4("view", view);
        mirrorCubeShader.setMat4("projection", projection);
        mirrorCubeShader.setVec3("cameraPos", camera.Position);
        
        glBindVertexArray(mirrorCubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

//....................skybox............................................................................................
        // рисуем skybox 
        glDepthFunc(GL_LEQUAL);  // фрагмент проходит тест, если его значение глубины меньше либо равно хранимому в буфере
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
//................bilbrords...........................................................................
        bilbordShader.Use();
        projection = glm::perspective(camera.Zoom, (float)NWIDTH / (float)NHEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        bilbordShader.setMat4("view", view);
        bilbordShader.setMat4("projection", projection);
        model = glm::mat4(1.0f);

        glBindVertexArray(bilbordVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bilbordTexture);
        for (std::map<float, glm::vec3>::reverse_iterator it = sortedBilbords.rbegin(); it != sortedBilbords.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            bilbordShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glfwSwapBuffers(window);
   
    }
    glDeleteVertexArrays(1, &ourcubeVAO);
    glDeleteBuffers(1, &ourcubeVBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &bilbordVAO);
    glDeleteBuffers(1, &bilbordVBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteVertexArrays(1, &mirrorCubeVAO);
    glDeleteBuffers(1, &mirrorCubeVBO);
    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // смотрим нажатые и отпущенные камеры
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void update_camera() {
    float cameraSpeed = 0.5 * deltaTime;
    if (keys[GLFW_KEY_W]) {
        camera.Keyboard(FORWARD, deltaTime);
    }
    if (keys[GLFW_KEY_S]) {
        camera.Keyboard(BACKWARD, deltaTime);
    }
    if (keys[GLFW_KEY_A]) {
        camera.Keyboard(LEFT, deltaTime);
    }
    if (keys[GLFW_KEY_D]) {
        camera.Keyboard(RIGHT, deltaTime);
    }

}


void mouse_moving(GLFWwindow* window, double xpos, double ypos) { // xpos,ypos - текущие координаты 
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 

    lastX = xpos;
    lastY = ypos;

    camera.MouseMovement(xoffset, yoffset);
}

void scroll_moving(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.MouseScrolling(yoffset);
}
void processInput(GLFWwindow* window)
{
    switch (GLFW_PRESS)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
    case GLFW_KEY_W:
        camera.Keyboard(FORWARD, deltaTime);
        break;
    case GLFW_KEY_S: 
        camera.Keyboard(BACKWARD, deltaTime);
        break;
    case GLFW_KEY_A:
        camera.Keyboard(LEFT, deltaTime);
        break;
    case GLFW_KEY_D:
        camera.Keyboard(RIGHT, deltaTime);
        break;
    default:
        break;
    }
 
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (heightScale > 0.0f)
            heightScale -= 0.0005f;
        else
            heightScale = 0.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if (heightScale < 1.0f)
            heightScale += 0.0005f;
        else
            heightScale = 1.0f;
    }
 
}


unsigned int loadTexture(char const* path)
{

    // создание идентификатора структуры
    unsigned int ourtexture;
    glGenTextures(1, &ourtexture);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;  // nrComponents  количестве цветовых каналов получившегося изображения
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        // связывание текстуры
        glBindTexture(GL_TEXTURE_2D, ourtexture);
        // сгенерируем текстуру
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // создаем все необходимые мипмап-уровни дя текстуры
        glGenerateMipmap(GL_TEXTURE_2D);
        //устанавливаем параметры наложения
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // параметры фильтрации
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    else
    {  // ошибочка
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    // освобождение памяти
    stbi_image_free(data);
    return ourtexture;
}

unsigned int loadCubemap(std::vector<std::string> pictures)
{
    // связывание текстуры
    unsigned int ourtexture;
    glGenTextures(1, &ourtexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ourtexture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < pictures.size(); i++)
    {
        unsigned char* pic = stbi_load(pictures[i].c_str(), &width, &height, &nrChannels, 0);
        if (pic)
        {
            // сгенерируем текстуру
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pic);
        }
        else
        {   // ошибочка
            std::cout << "Cubemap texture failed to load at path: " << pictures[i] << std::endl;
        }
        stbi_image_free(pic);
    }
    // настройка режимов текстурной фильтрации и повторения:
    // GL_TEXTURE_WRAP_R настраивает режим повторения по третьей координате текстуры
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return ourtexture;
}


unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        // координаты вершин
        glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3(1.0f, -1.0f, 0.0f);
        glm::vec3 pos4(1.0f, 1.0f, 0.0f);
        // текстурные координаты
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        // вектор нормали
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
 //..........1 треугольник...................................................    
        // вектора описывающие грани треугольника
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        // вектора описывающие дельты текстурных координат
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        // обозначим знаменатель для удобства
        float znam = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        // посчитаем касательную по формуле
        tangent1.x = znam * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = znam * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = znam * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);
        // посчитаем бикасательную по формуле
        bitangent1.x = znam * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = znam * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = znam * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent1 = glm::normalize(bitangent1);
 //..........2 треугольник................................................... 
       // вектора описывающие грани треугольника
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        // вектора описывающие дельты текстурных координат
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        znam = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = znam * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = znam * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = znam * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent2 = glm::normalize(tangent2);

        bitangent2.x = znam * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = znam * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = znam * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent2 = glm::normalize(bitangent2);


        float quadVertices[] = {
                                    // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

