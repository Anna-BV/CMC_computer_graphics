#define GLEW_STATIC
#include <GLEW/glew.h>

#include  <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <SOIL/SOIL.h>

#include "shader_h.h"
#include "camera_h.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void update_camera();
void mouse_moving(GLFWwindow* window, double xpos, double ypos);
void scroll_moving(GLFWwindow* window, double xoffset, double yoffset);

const GLuint NWIDTH = 800, NHEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
bool firstMouse = true;

GLfloat lastTime = 0.0f; // ¬рем€ вывода последнего кадра
GLfloat deltaTime = 0.0f;//  ¬рем€, прошедшее между последним и текущим кадром

GLfloat lastX = NWIDTH / 2.0;
GLfloat lastY = NHEIGHT / 2.0;
// положение источника света
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

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
    // ќткрыть окно и создать в нем контекст OpenGL
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

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, NWIDTH, NHEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader myShader("vertex_shader.vs", "fragment_shader.vs");
    Shader lightShader("light_vertex_shader.vs", "light_fragment_shader.vs");
    /*GLfloat vertices[] = { // ¬≈–Ў»Ќџ “–≈”√ќЋ№Ќ» ќ¬
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top  
    };*/
    GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f,  
    };
    
    // получим позицию камеры  !!! этот вектор фактически указывает в сторону, противоположную направлению камеры.
    //glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 4.0f);
    // камера нацелена в базовую точку сцены
    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // направление камеры
    //glm::vec3 cameraDirection = glm::vec3(cameraPosition - cameraTarget);
    //  вектор указывающий направление вверх 
    //glm::vec3 upvec = glm::vec3(0.0f, 1.0f, 0.0f);
    // найдем векторное произведение и получим вектор перпендикул€рный  - положительное направление оси ’
    //glm::vec3 cameraRight = glm::normalize(glm::cross(upvec, cameraDirection));
    // ось камеры вверх
    //glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
    
    /*GLuint indices[] = {  // ѕќ–яƒќ  –»—ќ¬јЌ»я
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };*/

    GLuint  VBO,containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    // св€зываем объект вершинного массива
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(containerVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    // Color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    // TexCoord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(2);
    
    // лампа
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // настройка атрибутов
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

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
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;

        glfwPollEvents();
        update_camera();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.Use();
        GLint objectColorLoc = glGetUniformLocation(myShader.Program, "objectColor");
        GLint lightColorLoc = glGetUniformLocation(myShader.Program, "lightColor");
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f); // зададим цвет объекта
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // зададим цвет источника света


        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"), 1);*/

        //glm::mat4 model;
        //GLfloat Radius = 10.0f;
        //GLfloat cameraXcoord = sin(glfwGetTime()) * Radius; // координаты X и Z - точки, расположенные на круговой траектории
        //GLfloat cameraZcoord = cos(glfwGetTime()) * Radius
        glm::mat4 view;
        /*view = glm::lookAt(glm::vec3(cameraXcoord, 0.0f, cameraZcoord), // позици€ камеры, вектор направленный вверх.
                           glm::vec3(0.0f, 0.0f, 0.0f), // координата цели
                           glm::vec3(0.0f, 1.0f, 0.0f)); //  вектор направленный вверх. */
        view = camera.GetViewMatrix();
        glm::mat4 projection;
        //projection = glm::perspective(45.0f, GLfloat(NWIDTH) / GLfloat(NHEIGHT), 0.1f, 100.0f);
        projection = glm::perspective(camera.Zoom, (GLfloat)NWIDTH / (GLfloat)NHEIGHT, 0.1f, 100.0f);
        GLint modelLoc = glGetUniformLocation(lightShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); 
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(containerVAO);
        glm::mat4 model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        lightShader.Use();
        modelLoc = glGetUniformLocation(lightShader.Program, "model");
        viewLoc = glGetUniformLocation(lightShader.Program, "view");
        projLoc = glGetUniformLocation(lightShader.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4();
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        //if ((i + 1) % 2 == 1) {
        //    model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        //}
        // рисование куба лампы 
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &containerVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
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
    GLfloat cameraSpeed = 0.5*deltaTime;
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
    GLfloat yoffset = lastY - ypos; // обратный пор€док вычитани€ потому что оконные Y-координаты возрастают с верху вниз 
   
    lastX = xpos;
    lastY = ypos;
    
    camera.MouseMovement(xoffset, yoffset);
}

void scroll_moving(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.MouseScrolling(yoffset);
}