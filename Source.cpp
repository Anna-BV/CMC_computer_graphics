#define GLEW_STATIC
//#include <GLEW/glew.h>
#include <stb_image/stb_image.h>
#include <GLAD/include/glad/glad.h>
#include  <GLFW/glfw3.h>
#include <string>
#include <iostream>
//#include <SOIL/SOIL.h>


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

const GLuint NWIDTH = 800, NHEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
bool firstMouse = true;

float lastTime = 0.0f; // Время вывода последнего кадра
float deltaTime = 0.0f;//  Время, прошедшее между последним и текущим кадром

float lastX = NWIDTH / 2.0;
float lastY = NHEIGHT / 2.0;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f); // 
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

    /*glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    } */

    //glViewport(0, 0, NWIDTH, NHEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader lightShader("light_vertex_shader.vs", "light_fragment_shader.vs"); // замена шейдеров
    //std::cout << "myShader" << std::endl;
    Shader myShader("all_light.vs", "all_light.fs");
    //std::cout << "lightShader" << std::endl;
    /*GLfloat vertices[] = { // ВЕРШИНЫ ТРЕУГОЛЬНИКОВ
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top
    };*/

    GLfloat vertices[] = {
        // нормали
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
    // задаем положения кубов
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 pointLightPositions[] = {
       glm::vec3(0.7f,  0.2f,  2.0f),
       glm::vec3(2.3f, -3.3f, -4.0f),
       glm::vec3(0.0f,  0.0f, -3.0f)
    };
    glm::vec3 pointLightColors[] = {
          glm::vec3(0.2f, 0.2f, 0.6f),
          glm::vec3(0.9f, 0.3f, 0.86f),
          glm::vec3(0.9f, 0.4f, 0.9f),
    };


    GLuint  VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    // связываем объект вершинного массива


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(containerVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    //glBindVertexArray(0);
    // лампа
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // настройка атрибутов
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0));
    glEnableVertexAttribArray(0);


    unsigned int diffuseMap = loadTexture("box.png");
    unsigned int specularMap = loadTexture("container2_specular.png");

    myShader.Use();
    myShader.setInt("material.diffuse", 0);
    myShader.setInt("material.specular", 1);
    /*GLuint texture1;
    GLuint texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image("stone.png", &width, &height, 0, SOIL_LOAD_RGB);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); */

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;

        processInput(window);

        //glClearColor(1.0f, 0.89f, 0.92f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.Use();
        myShader.setVec3("viewPos", camera.Position);
        myShader.setFloat("material.shininess", 32.0f);
        // направленный источник
        myShader.setVec3("directLight.ambient", 0.0f, 0.0f, 0.0f);
        myShader.setVec3("directLight.diffuse", 0.2f, 0.2f, 0.2f);
        myShader.setVec3("directLight.specular", 0.5f, 0.5f, 0.5f);
        myShader.setVec3("directLight.direction", -0.2f, -1.0f, -0.3f);
        // точечный источник 1
        myShader.setVec3("pointLight[0].position", pointLightPositions[0]);
        myShader.setVec3("pointLight[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
        myShader.setVec3("pointLight[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        myShader.setVec3("pointLight[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        myShader.setFloat("pointLight[0].constant", 1.0f);
        myShader.setFloat("pointLight[0].linear", 0.09);
        myShader.setFloat("pointLight[0].quadratic", 0.08);
        // точечный источник 2
        myShader.setVec3("pointLight[1].position", pointLightPositions[1]);
        myShader.setVec3("pointLight[1].ambient", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        myShader.setVec3("pointLight[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        myShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        myShader.setFloat("pointLight[1].constant", 1.0f);
        myShader.setFloat("pointLight[1].linear", 0.09);
        myShader.setFloat("pointLight[1].quadratic", 0.08);

        myShader.setVec3("pointLight[2].position", pointLightPositions[2]);
        myShader.setVec3("pointLight[2].ambient", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        myShader.setVec3("pointLight[1].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        myShader.setVec3("pointLights[1].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        myShader.setFloat("pointLight[1].constant", 1.0f);
        myShader.setFloat("pointLight[1].linear", 0.07);
        myShader.setFloat("pointLight[1].quadratic", 0.05);

        // прожектор
        myShader.setVec3("spotLight.position", camera.Position);
        myShader.setVec3("spotLight.direction", camera.Front);
        myShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        myShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        myShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("spotLight.constant", 1.0f);
        myShader.setFloat("spotLight.linear", 0.09);
        myShader.setFloat("spotLight.quadratic", 0.032);
        myShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(13.5f)));
        myShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.0f)));



        glm::mat4 view;
        glm::mat4 projection;
        projection = glm::perspective(camera.Zoom, (float)NWIDTH / (float)NHEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        myShader.setMat4("projection", projection);
        myShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        myShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(containerVAO);
        for (unsigned int i = 0; i < 8; i++) {

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            myShader.setMat4("model", model);
            //glBindVertexArray(containerVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

        lightShader.Use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
       
        
        glBindVertexArray(lightVAO);
        for (int i = 0; i < 3; i++) {
            lightShader.setVec3("colorpop", pointLightColors[i]);
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            lightShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &containerVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //std::cout << key << std::endl;
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
    GLfloat cameraSpeed = 0.5 * deltaTime;
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

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Keyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Keyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Keyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Keyboard(RIGHT, deltaTime);
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
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    // освобождение памяти
    stbi_image_free(data);
    return ourtexture;
}