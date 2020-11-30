#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
const unsigned int our_width = 640;
const unsigned int out_height = 480;

void processInput(GLFWwindow* window) { // обработка событий вводы
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
    //glfwInit();
    // »нициализируем GLFW и конфигурирование 
    if (!glfwInit()) {
        fprintf(stderr, "ќшибка при инициализации GLFWn");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;

    // ќткрыть окно и создать в нем контекст OpenGL
    window = glfwCreateWindow(our_width,out_height, "CMC_computer_graphics", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Ќевозможно открыть окно");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glad: загрузка указателей на OpenGL функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD");
        return -1;
    }
    //glViewport(0, 0, 640, 480);
    // создадим вершинный шейдер
    /*unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // проверка на наличие ошибок
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    // фрагментный шейдер
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // проверка на наличие ошибок
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    // создаем объект шейдерной программы
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // прикрепл€ем шейдеры к объекту программы и соедин€ем
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // проверка на наличие ошибок
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    // удал€ем шейдерные объекты
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    */
    Shader myShader("C:/с++/Project1/shader.vs", "C:/с++/Project1/shader.fs");
    float vertices[] = {    // вершины
       0.5f,  0.5f, 0.0f,   1.0f,0.0f,0.0f,    1.0f,1.0f,        // верхн€€ права€
       0.5f, -0.5f, 0.0f,   0.0f,1.0f,0.0f,    1.0f,0.0f,        // нижн€€ права€
      -0.5f, -0.5f, 0.0f,   0.0f,0.0f,1.0f,    0.0f,0.0f,         // нижн€€ лева€
      -0.5f,  0.5f, 0.0f,   1.0f,1.0f,0.0f,    0.0f,1.0f         // верхн€€ лева€  
    };
   unsigned int indices[] = { // пор€док
        0,1,3,
        1,2,3
    }; 
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // св€зываем объект вершинного массива
    glBindVertexArray(VAO);
    // копируем наш массив вершин в буфер
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // атрибуты координат
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    // атрибуты цвета
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // атрибуты текстуры
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2); 

    
    
    // создание идентификатора структуры
    unsigned int first_texture;
    glGenTextures(1, &first_texture);
    // св€зывание текстуры
    glBindTexture(GL_TEXTURE_2D, first_texture);
    //устанавливаем параметры наложени€
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // параметры фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height; // задаем ширину и длину
    int number_of_channels;
    unsigned char* pic = stbi_load("C:/с++/Project1/texture/stone_texture.jpg", &width, &height, &number_of_channels, 0);
    if (pic) {
            // сгенерируем текстуру
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pic);
            // используем мипмап текстуры
            glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed in loading texture" << std::endl;
    }
    // освобождение пам€ти
    stbi_image_free(pic);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.6f, 0.0f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, first_texture);

        myShader.use();
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // завершение,освобождение всех задействованных ресурсов
    glfwTerminate();
    return 0;
}