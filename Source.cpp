#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void processInput(GLFWwindow* window) { // обработка событий вводы
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
// вершинный шейдер
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = vec4(0.5,0.5,0.5,1.0);\n"
"}\0";
// фрагментный шейдер
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;" // передано
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

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
    window = glfwCreateWindow(640, 480, "CMC_computer_graphics", NULL, NULL);
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
    glViewport(0, 0, 640, 480);
    // создадим вершинный шейдер
    unsigned int vertexShader;
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

    float vertices[] = {    // вершины
       0.5f,  0.5f, 0.0f,  // верхн€€ права€
       0.5f, -0.5f, 0.0f,  // нижн€€ права€
       -0.5f, -0.5f, 0.0f,  // нижн€€ лева€
       -0.5f,  0.5f, 0.0f   // верхн€€ лева€  
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
    // укаатели вершинных атрибутов
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.6f, 0.0f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /*
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.6f, 0.0f, 0.9f, 1.0f);
        glBegin(GL_QUADS);

        // лева€ грань
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        // права€ грань
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f,-0.5f);
        // нижн€€ грань
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        // верхн€€ грань
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        // задн€€ грань
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // передн€€ грань
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();
        //glBegin(GL_TRIANGLES);
        //glVertex2f(-0.5f, -0.5f);
        //glVertex2f(0.0f, 0.5f);
        //glVertex2f(0.5f, -0.5f);
        //glEnd();

        // —брасываем буферы
        glfwSwapBuffers(window);
        glfwPollEvents();
    } */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // завершение,освобождение всех задействованных ресурсов
    glfwTerminate();
    return 0;
}